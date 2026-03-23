#pragma once
#include <Arduino.h>
#include <functional>
#include <queue>

#include "Dali/Frame.h"
#include "Dali/Receiver/Base.h"
#include "Dali/Transmitter/Base.h"

namespace Dali
{
    class DataLinkLayer
    {
        friend class Receiver::Base;

        std::vector<std::function<void(Frame)>> _callbackMonitors;

        std::queue<Frame> _txQueue;
        std::queue<Frame> _rxQueue;

        Frame _txFrame;

        Transmitter::Base *_txTransmitter = nullptr;
        Receiver::Base *_rxReceiver = nullptr;

        bool _nextResponse = false;
        unsigned long _nextResponseTimer = 0;
        unsigned long _lastFrameReceived = 0;

        void processReceive();
        void processTransmit();
        void receivedFrame(Frame frame);

      public:
        bool transmitFrame(Frame frame);
        void init(uint tx, uint rx);
        void process();
        void registerMonitor(std::function<void(Frame)> callback);
        bool receiving();
        bool transmitting();
        bool busy();
        unsigned long &lastFrameReceived();
        bool connected();
        size_t transmitQueueSize();
        size_t receiveQueueSize();

    };

}; // namespace Dali
