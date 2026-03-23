#pragma once
#include "Dali/Frame.h"
#include "Dali/defines.h"
#include <Arduino.h>

namespace Dali
{
    class DataLinkLayer;

    namespace Receiver
    {
        class Base
        {
            friend class DataLinkLayer;

          protected:
            uint _pin;
            bool _receiving = false;
            unsigned long _lastTime = 0;
            DataLinkLayer *_dll;

            void startReceiving();
            void expireReceiving();
            void receivedFrame(Frame frame);

          public:
            Base(DataLinkLayer *dll, uint pin) : _dll(dll), _pin(pin) {}
            bool receiving();
            unsigned long lastReceiving();
            virtual void process();
            uint pin() { return _pin; };
            virtual bool connected();
        };
    } // namespace Receiver

} // namespace Dali