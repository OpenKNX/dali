#pragma once
#include "Dali/Frame.h"
#include "Dali/defines.h"
#include <Arduino.h>

namespace Dali
{
    class DataLinkLayer;

    namespace Transmitter
    {
        class Base
        {
            friend class DataLinkLayer;

          protected:
            uint _pin;
            bool _transmitting = false;
            unsigned long _lastTime = 0;
            DataLinkLayer *_dll;

          public:
            Base(DataLinkLayer *dll, uint pin) : _dll(dll), _pin(pin) {}
            virtual void transmitFrame(Frame frame) = 0;
            virtual void process();
            bool transmitting();
            void transmitting(bool transmitting);
            void expireTransmitting();
            uint pin() { return _pin; };
        };
    } // namespace Transmitter

} // namespace Dali