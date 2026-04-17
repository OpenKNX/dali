#pragma once
#ifdef ARDUINO_ARCH_RP2040
#include <Arduino.h>
#include <array>
#include <functional>
#include "hardware/pio.h"

#ifndef DALI_PIO
#define DALI_PIO pio0
#endif

namespace Dali
{
    namespace Pio
    {
        class Manager
        {
            PIO _pio = DALI_PIO;

            uint _rxProgramOffset;
            uint _txProgramOffset;
            uint _rxStateMachineCounter = 0;
            uint _txStateMachineCounter = 0;

#if defined(PICO_RP2350)
            irq_num_rp2350 _irq;
#else
            irq_num_rp2040 _irq;
#endif

            std::array<std::function<void()>, 4> _interruptHandlers;

          public:
            Manager();
            PIO &pio();
            uint rxProgramOffset();
            uint txProgramOffset();
            int rxStateMachine();
            int txStateMachine();
            inline std::function<void()> interruptHandler(uint idx);
            void setInterruptHandler(int index, std::function<void()> handler);
            static void __not_in_flash_func(interrupt)();
        };
    } // namespace Pio
} // namespace Dali

extern Dali::Pio::Manager DaliPioManager;
#endif