#ifdef ARDUINO_ARCH_ESP32
#include "Dali/Receiver/Rmt.h"
namespace Dali
{
    namespace Receiver
    {
        Rmt::Rmt(DataLinkLayer *dll, uint pin) : Base(dll, pin)
        {
            // digitalWrite(8, HIGH);
            sprintf(_taskName, "%s%d", "dalirmtrx", pin);
            _channelConfig = (rmt_rx_channel_config_t){
                .gpio_num = (gpio_num_t)pin,
                .clk_src = RMT_CLK_SRC_DEFAULT,
                .resolution_hz = DALI_RMT_RESOLUTION_HZ,
                .mem_block_symbols = DALI_RX_BITS, // amount of RMT symbols that the channel can store at a time
                //.intr_priority = 2
            };

            _receiveConfig = (rmt_receive_config_t){
                .signal_range_min_ns = 2000,
                .signal_range_max_ns = (uint32_t)(DALI_TE * 4 * 1000)};

            rmt_rx_event_callbacks_t callback = {
                .on_recv_done = Rmt::callback,
            };

            _queueHandle = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));

            xTaskCreate(Rmt::task, _taskName, 4096, this, 0, &_taskHandle);
            ESP_ERROR_CHECK(rmt_new_rx_channel(&_channelConfig, &_channelHandle));
            ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(_channelHandle, &callback, _queueHandle));
            ESP_ERROR_CHECK(rmt_enable(_channelHandle));

            // Start receiving
            pinMode(pin, INPUT_PULLUP);
            rmt_receive(_channelHandle, _symbols, sizeof(_symbols), &_receiveConfig);
            attachInterruptArg(pin, Rmt::interrupt, this, CHANGE);
        }

        bool Rmt::callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data)
        {

            BaseType_t high_task_wakeup = pdFALSE;
            QueueHandle_t _queueHandle = (QueueHandle_t)user_data;
            xQueueSendFromISR(_queueHandle, edata, &high_task_wakeup);
            return high_task_wakeup == pdTRUE;
        }

        void Rmt::interrupt(void *pvParameters)
        {
            Rmt *receiver = static_cast<Rmt *>(pvParameters);
            // digitalWrite(8, LOW);
            // Serial.println("I2");
            receiver->startReceiving();
        }

        QueueHandle_t Rmt::getQueueHandle()
        {
            return _queueHandle;
        }

        void Rmt::task(void *pvParameters)
        {
            Rmt *receiver = static_cast<Rmt *>(pvParameters);
            rmt_rx_done_event_data_t data;

            while (true)
            {
                if (xQueueReceive(receiver->getQueueHandle(), &data, portMAX_DELAY) == pdPASS)
                {
                    receiver->stopReceiving(data);
                }
                vTaskDelay(1);
            }
        }

        void Rmt::stopReceiving(rmt_rx_done_event_data_t data)
        {
            // restart receiving
            rmt_receive(_channelHandle, _symbols, sizeof(_symbols), &_receiveConfig);

            Frame frame;

            bool error = false;
            size_t counter = 0;

            for (int i = 0; i < data.num_symbols; i++)
            {
                size_t highs = round((float)data.received_symbols[i].duration0 / 417);
                size_t lows = round((float)data.received_symbols[i].duration1 / 417);

                // special for bit (stop bits)
                if (data.received_symbols[i].duration1 == 0) lows = 1;

                // start bit must be 1
                if (i == 0 && highs != 1) error = true;

                // menchester coding allows only 1 or 2 halbits are same
                if (highs < 1 || highs > 2) error = true;
                if (lows < 1 || lows > 2) error = true;

                // skip on error
                if (error)
                {
                    printf("Error (%u)\n", counter);
                    for (size_t i = 0; i < data.num_symbols; i++)
                    {
                        printf(" - %u: %u (%u) %u (%u)\n", i, data.received_symbols[i].duration0, data.received_symbols[i].level0, data.received_symbols[i].duration1, data.received_symbols[i].level1);
                    }
                    break;
                }

                for (int j = 0; j < highs; j++)
                {
                    if (counter > 1 && counter % 2)
                    {
                        frame.size++;
                        frame.data <<= 1;
                    }
                    counter++;
                }

                for (int j = 0; j < lows; j++)
                {
                    if (counter > 1 && counter % 2)
                    {
                        frame.size++;
                        frame.data <<= 1;
                        frame.data |= 1;
                    }
                    counter++;
                }
            }

            if (error)
            {
                frame.size = 0;
                frame.data = 0;
                frame.flags = DALI_FRAME_ERROR;
            }
            // digitalWrite(8, LOW);
            // Serial.printf("Rx<%u>: %u: Frame done\n", pin(), micros());
            _lastTime = micros();
            receivedFrame(frame);
        }

    } // namespace Receiver

} // namespace Dali
#endif