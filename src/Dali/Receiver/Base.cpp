#include "Dali/Receiver/Base.h"
#include "Dali/DataLinkLayer.h"

namespace Dali
{
    namespace Receiver
    {
        bool Base::receiving()
        {
            return _receiving;
        }

        void Base::process()
        {
            expireReceiving();
        }

        void Base::expireReceiving()
        {
            if (!receiving()) return;

            // (1 start + 32 data + 2 stop) * 2 TE + 20 TE pause/reserve
            // in this time the frame should be received
            const unsigned long last = _lastTime;
            const unsigned long current = micros();
            if (current - last < DALI_TE_TO_US(90)) return;

            _receiving = false;
            // Serial.printf("Rx<%u>: %u | %u - %u: Expire Receiving \n", _pin, micros(), current , last);
        }

        void Base::receivedFrame(Frame frame)
        {
            _lastTime = micros();
            asm volatile("" ::: "memory");
            _receiving = false;
            // Serial.printf("Rx<%u>: %u: Receiving complete: %u \n", _pin, micros(), _receiving);

            frame.timestamp = _lastTime;
            _dll->receivedFrame(frame);
        }

        void Base::startReceiving()
        {
            _lastTime = micros();
            asm volatile("" ::: "memory");

            if (_receiving) return;
            _receiving = true;
            // Serial.printf("Rx<%u>: %u: Start Receiving \n", _pin, micros());
        }

        unsigned long Base::lastReceiving()
        {
            return _lastTime;
        }

        // TODO
        bool Base::connected()
        {
            return true;
        }
    } // namespace Receiver
} // namespace Dali