#ifdef ARDUINO_ARCH_RP2040
#include "Dali/Transmitter/Pio.h"
#include "Dali/Pio/StateMachines.h"

namespace Dali
{
    namespace Transmitter
    {
        Pio::Pio(DataLinkLayer *dll, uint pin) : Base(dll, pin)
        {
            _sm = DaliPioManager.txStateMachine();
            dali_tx_program_init(DaliPioManager.pio(), _sm, DaliPioManager.txProgramOffset(), pin);
        }

        void Pio::transmitFrame(Frame frame)
        {
            // Serial.printf("Transmit: 0x%08X S: %u\n", frame.data, frame.size);
            while (!pio_sm_is_tx_fifo_empty(DaliPioManager.pio(), _sm))
                delay(1);

            uint32_t mask = (1u << frame.size) - 1;
            uint32_t lower_bits = frame.data & mask;
            uint32_t reversed = 0;
            for (uint8_t i = 0; i < frame.size; i++)
            {
                reversed = (reversed << 1) | (lower_bits & 1);
                lower_bits >>= 1;
            }

            pio_sm_put(DaliPioManager.pio(), _sm, frame.size);
            pio_sm_put(DaliPioManager.pio(), _sm, reversed);
            transmitting(true);
        }
    } // namespace Transmitter
} // namespace Dali
#endif