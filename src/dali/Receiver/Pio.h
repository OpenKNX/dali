#pragma once
#ifdef ARDUINO_ARCH_RP2040
#include "Dali/Pio/Manager.h"
#include "Dali/Receiver/Base.h"

namespace Dali
{
    class DataLinkLayer;

    namespace Receiver
    {
        class Pio : public Base
        {
            uint _sm;
            char buffer[DALI_RX_BITS];
            size_t _zeros = 2;
            size_t bufferIndex = 0;
            Frame _frame;

          public:
            Pio(Dali::DataLinkLayer *dll, uint pin);
            void __not_in_flash_func(interrupt)();
            void process() override;
        };
    } // namespace Receiver
} // namespace Dali
#endif