#include "Dali/Receiver/Base.h"
#include "Dali/DataLinkLayer.h"

namespace Dali
{
    namespace Transmitter
    {
        bool Base::transmitting()
        {
            return _transmitting;
        }

        void Base::process()
        {
            expireTransmitting();
        }

        void Base::expireTransmitting()
        {
            if (!transmitting()) return;

            // (1 start + 32 data + 2 stop) * 2 TE + 20 TE pause/reserve
            // in this time the transmitted frame should be received
            if (micros() - _lastTime < DALI_TE_TO_US(90)) return;
            // Serial.printf("%u: Expire Transmitting \n", micros());

            transmitting(false);
        }

        void Base::transmitting(bool transmitting)
        {
            _transmitting = transmitting;
            if (transmitting) _lastTime = micros();
            // Serial.printf("%u: Transmitting: %u \n", micros(), _transmitting);
        }
    } // namespace Transmitter
} // namespace Dali