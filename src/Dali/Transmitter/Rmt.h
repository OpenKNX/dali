#pragma once
#ifdef ARDUINO_ARCH_ESP32
#include "Dali/Transmitter/Base.h"
#include "Dali/defines.h"
#include "driver/rmt_tx.h"

namespace Dali
{
    namespace Transmitter
    {
        class Rmt : public Base
        {
            rmt_channel_handle_t _channelHandle;
            rmt_tx_channel_config_t _channelConfig;
            rmt_transmit_config_t _transmitConfig;

          public:
            Rmt(DataLinkLayer *dll, uint pin);
            void transmitFrame(Frame frame) override;
            void encode(Frame frame, rmt_symbol_word_t *symbols);
        };
    } // namespace Transmitter

} // namespace Dali
#endif