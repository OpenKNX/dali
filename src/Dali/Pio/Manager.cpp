#ifdef ARDUINO_ARCH_RP2040
#include "Dali/Pio/Manager.h"
#include "Dali/Pio/StateMachines.h"

namespace Dali
{
    namespace Pio
    {
        Manager::Manager()
        {
            _txProgramOffset = pio_add_program(_pio, &dali_tx_program);
            _rxProgramOffset = pio_add_program(_pio, &dali_rx_program);

            _irq = (_pio == pio1) ? PIO1_IRQ_0 : PIO0_IRQ_0;

            pio_set_irq0_source_enabled(_pio, pis_interrupt0, true);
            pio_set_irq0_source_enabled(_pio, pis_interrupt1, true);
            pio_set_irq0_source_enabled(_pio, pis_interrupt2, true);
            pio_set_irq0_source_enabled(_pio, pis_interrupt3, true);

            irq_set_exclusive_handler(_irq, Manager::interrupt);
            irq_set_enabled(_irq, true);
        }

        void Manager::setInterruptHandler(int index, std::function<void()> handler)
        {
            _interruptHandlers[index] = handler;
        }

        uint Manager::rxProgramOffset()
        {
            return _rxProgramOffset;
        }

        uint Manager::txProgramOffset()
        {
            return _txProgramOffset;
        }

        int Manager::rxStateMachine()
        {
            if (_rxStateMachineCounter > 2) return -1;
            return _rxStateMachineCounter++;
        }

        int Manager::txStateMachine()
        {
            if (_txStateMachineCounter > 2) return -1;
            return _txStateMachineCounter++ + 2;
        }

        PIO &Manager::pio()
        {
            return _pio;
        }

        std::function<void()> Manager::interruptHandler(uint idx)
        {
            return _interruptHandlers[idx];
        }

        void Manager::interrupt()
        {
            for (int i = 0; i < 4; ++i)
            {
                if (pio_interrupt_get(DaliPioManager.pio(), i))
                {
                    pio_interrupt_clear(DaliPioManager.pio(), i);
                    std::function<void()> handler = DaliPioManager.interruptHandler(i);
                    if (handler) handler();
                }
            }
        }
    } // namespace Pio
} // namespace Dali

Dali::Pio::Manager DaliPioManager;
#endif