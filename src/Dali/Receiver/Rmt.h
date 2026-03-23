#pragma once
#ifdef ARDUINO_ARCH_ESP32
#include "Dali/Receiver/Base.h"
#include "Dali/defines.h"
#include "driver/rmt_rx.h"

// Umwandlung von TE (in Mikrosekunden) auf die Anzahl der RMT-Symbole
#define DALI_TE_TO_RMT(x) ((uint16_t)((x) * (DALI_RMT_RESOLUTION_HZ / 1000000.0)))

// DALI-Symbole
static const rmt_symbol_word_t DALI_SYMBOL_ONE = {
    .duration0 = DALI_TE_TO_RMT(DALI_TE),
    .level0 = 1,
    .duration1 = DALI_TE_TO_RMT(DALI_TE),
    .level1 = 0,
};

static const rmt_symbol_word_t DALI_SYMBOL_ZERO = {
    .duration0 = DALI_TE_TO_RMT(DALI_TE),
    .level0 = 0,
    .duration1 = DALI_TE_TO_RMT(DALI_TE),
    .level1 = 1,
};

static const rmt_symbol_word_t DALI_SYMBOL_STOP = {
    .duration0 = DALI_TE_TO_RMT(DALI_TE),
    .level0 = 0,
    .duration1 = DALI_TE_TO_RMT(DALI_TE),
    .level1 = 0,
};

namespace Dali
{
    class DataLinkLayer;

    namespace Receiver
    {

        class Rmt : public Base
        {
            char _taskName[20];
            rmt_symbol_word_t _symbols[DALI_RX_BITS];
            rmt_channel_handle_t _channelHandle;
            rmt_rx_channel_config_t _channelConfig;
            rmt_receive_config_t _receiveConfig;
            TaskHandle_t _taskHandle;
            QueueHandle_t _queueHandle;

          public:
            Rmt(DataLinkLayer *dll, uint pin);
            static void interrupt(void *pvParameters);
            static void task(void *pvParameters);
            static bool callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data);
            void stopReceiving(rmt_rx_done_event_data_t data);
            QueueHandle_t getQueueHandle();
        };

    } // namespace Receiver

} // namespace Dali
#endif