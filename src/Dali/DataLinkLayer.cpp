#include "Dali/DataLinkLayer.h"

#ifdef ARDUINO_ARCH_ESP32
#include "Dali/Receiver/Rmt.h"
#include "Dali/Transmitter/Rmt.h"
#endif
#ifdef ARDUINO_ARCH_RP2040
#include "Dali/Receiver/Pio.h"
#include "Dali/Transmitter/Pio.h"
#endif

namespace Dali
{
    bool DataLinkLayer::transmitFrame(Frame frame)
    {
        if (frame.size > 32) return false;
        frame.timestamp = micros();
        _txQueue.push(frame);
        return true;
    };

    // This function actually does more than it should—it takes on tasks typically handled by the network layer.
    void DataLinkLayer::receivedFrame(Frame frame)
    {

        // check it response is in expeted time frame of 22TE.
        if (_nextResponse)
        {
            unsigned long _startReceiving = (frame.size + 1 + 2) * DALI_TE_TO_US(2);
            if (_startReceiving <= _nextResponseTimer)
            {
                frame.flags |= DALI_FRAME_BACKWARD;
                frame.ref = _txFrame.ref;
                _nextResponse = false;
            }
            _nextResponse = false;
        }

        if (_txTransmitter->transmitting())
        {
            if (_txFrame.data == frame.data && _txFrame.size == frame.size)
            {
                frame.flags |= _txFrame.flags;  // apply flags from txFrame
                frame.flags |= DALI_FRAME_ECHO; // apply echo
                frame.ref = _txFrame.ref;       // apply ref
                _nextResponse = true;           // wait for possible response
                _nextResponseTimer = micros() + DALI_TE_TO_US(22);
            }
            else
            {
                // Serial.printf("Rx<%u>: %u: Frame collision %08X %08X\n", _txTransmitter->pin(), micros(), _txFrame.data, frame.data);
                frame.flags |= DALI_FRAME_COLLISION | DALI_FRAME_ERROR;
                frame.ref = _txFrame.ref;
                frame.data = 0;
                frame.size = 0;
            }
        }

        // Regardless of the message received, the transmission is always terminated afterward.
        _txTransmitter->transmitting(false);

        // Touch activity time
        _lastFrameReceived = frame.timestamp;

        // enqueue the received frame
        _rxQueue.push(frame);
    };

    void DataLinkLayer::init(uint tx, uint rx)
    {
        // Initialize the transmitter and receiver based on the architecture.
#ifdef ARDUINO_ARCH_ESP32
        _rxReceiver = new Dali::Receiver::Rmt(this, rx);
        _txTransmitter = new Dali::Transmitter::Rmt(this, tx);
#endif
#ifdef ARDUINO_ARCH_RP2040
        _rxReceiver = new Dali::Receiver::Pio(this, rx);
        _txTransmitter = new Dali::Transmitter::Pio(this, tx);
#endif
    };

    void DataLinkLayer::process()
    {
        _rxReceiver->process();
        processReceive();
        _rxReceiver->process();
        processReceive();
        processTransmit();
        _txTransmitter->process();
    };

    void DataLinkLayer::processReceive()
    {
        if (_rxQueue.empty()) return;

        Frame rxFrame = _rxQueue.front();
        _rxQueue.pop();

        // Serial.printf("Rx<%u>: %u: Frame 0x%08X (S: %u - F: %u)\n", _rxReceiver->pin(), micros(), rxFrame.data, rxFrame.size, rxFrame.flags);

        for (std::function<void(Frame)> &callback : _callbackMonitors)
            callback(rxFrame);
    }

    void DataLinkLayer::processTransmit()
    {
        if (_txQueue.empty()) return;

        if (_txTransmitter->transmitting()) return;

        Frame txFrame = _txQueue.front();
        if (txFrame.flags & DALI_FRAME_BACKWARD)
        {
            // Backward frames must be transmitted 7TE after the previous frame (up to a maximum of 21TE after receiving it) and can potentially cause collisions (e.g., in broadcast mode).
            // Therefore, instead of using the last recent timestamp, the timestamp from the previous frame is used. Since a frame already includes 4TE in the form of 2 stop bits, an additional 3TE is sufficient.
            const unsigned long diff = micros() - _lastFrameReceived;
            if (diff < DALI_TE_TO_US(3)) return;
            // Serial.printf("Tx<%u>: %u: Check Backward ok (%u)\n", _txTransmitter->pin(), micros(), diff);
        }
        else
        {
            if (_rxReceiver->receiving()) return;
            
            // forward frames should send after 22TE to allow receiving an answer
            const unsigned long diff = micros() - _rxReceiver->lastReceiving();
            if (diff < DALI_TE_TO_US(22)) return;
            // Serial.printf("%u: Check Forward ok (%u)\n", _txTransmitter->pin(), diff);
        }
        _txFrame = txFrame;
        _txQueue.pop();

        // Serial.printf("Tx<%u>: %u: Frame 0x%08X (S: %u - F: %u)\n", _txTransmitter->pin(), micros(), _txFrame.data, _txFrame.size, _txFrame.flags);
        _txTransmitter->transmitFrame(_txFrame);
        _nextResponse = false;
    }

    void DataLinkLayer::registerMonitor(std::function<void(Frame)> callback)
    {
        _callbackMonitors.push_back(callback);
    }

    bool DataLinkLayer::receiving()
    {
        return _rxReceiver->receiving();
    }

    bool DataLinkLayer::transmitting()
    {
        return _txTransmitter->transmitting();
    }

    bool DataLinkLayer::busy()
    {
        return _txQueue.size() > 0 || _txTransmitter->transmitting() || _rxReceiver->receiving();
    }

    size_t DataLinkLayer::transmitQueueSize()
    {
        return _txQueue.size();
    }

    size_t DataLinkLayer::receiveQueueSize()
    {
        return _rxQueue.size();
    }

    unsigned long &DataLinkLayer::lastFrameReceived()
    {
        return _lastFrameReceived;
    }

    bool DataLinkLayer::connected()
    {
        return _rxReceiver->connected();
    }
}; // namespace Dali
