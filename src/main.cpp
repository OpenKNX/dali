#ifdef DALI_LIB_TEST

#include "Arduino.h"
#include "Dali/Master.h"

Dali::Master m1;
Dali::Master m2;

uint32_t inttime = 0;
uint8_t tdata;
bool tstatus = false;
uint32_t lasttx = 0;

#ifdef ARDUINO_ARCH_RP2040
#define printf Serial.printf
#endif

void onDaliFrame1(Dali::Frame frame)
{
    // if (!(frame.flags & DALI_FRAME_ECHO))
    // {
    //     Dali::Frame txFrame;
    //     txFrame.flags = DALI_FRAME_BACKWARD;
    //     txFrame.data = frame.data & 0x000000FF;
    //     txFrame.size = 8;
    //     dll1.transmitFrame(txFrame);
    // }
    // digitalWrite(8, HIGH);
    printf("Monitor1: 0x%08X (S: %u - F: %u - T: %u - R: %u)\n", frame.data, frame.size, frame.flags, frame.timestamp, frame.ref);
}
void onDaliFrame2(Dali::Frame frame)
{
    printf("Monitor2: 0x%08X (S: %u - F: %u)\n", frame.data, frame.size, frame.flags);
    // if (frame.data == 0x000010FF)
    // {
    //     // printf("Generate2\n");
    //     Dali::Frame txFrame;
    //     txFrame.flags = DALI_FRAME_BACKWARD;
    //     txFrame.data = frame.data & 0x000000FF;
    //     txFrame.size = 8;
    //     dll2.transmitFrame(txFrame);
    // }
    // else
    // {
    //     // printf("No Generate2\n");
    // }
}

void setup()
{
    // pinMode(8, OUTPUT);
    // neue zeile
    Serial.begin(115200);
    delay(5000);
    printf("Setup\n");
#ifdef ARDUINO_ARCH_ESP32
    m1.init(7, 20);
    m2.init(5, 8);
#endif
#ifdef ARDUINO_ARCH_RP2040
    m1.init(17, 16);
    m2.init(26, 18);
#endif
    m1.registerMonitor(onDaliFrame1);
    m2.registerMonitor(onDaliFrame2);
    printf("Setup done\n");
}

uint32_t last = 0;
uint8_t addr = 0;
uint8_t state = 0; // 0 = query; 1 = waiting, 2 = delay
uint32_t ref = 0;

void loop()
{
    m1.process();
    m2.process();

    // if(millis() - last > 6000)
    // {
    //     Dali::Frame txFrame;
    //     txFrame.flags = DALI_FRAME_FORWARD;
    //     txFrame.data = 0x000000FF;
    //     txFrame.size = 8;
    //     m1.sendCommand(0, Dali::Command::OFF, false, true);
    //     last = millis();
    // }

    if(state == 0)
    {
        // we send a query level here
        printf("Query level A%i\n", addr);
        ref = m1.sendCommand(addr, Dali::QUERY_ACTUAL_LEVEL, false, true);
        state = 1;
    } else if(state == 1)
    {
        Dali::Response r = m1.getResponse(ref);
        if(r.state == Dali::ResponseState::RECEIVED)
        {
            printf("Response A%i: %u (F: %u)\n", addr, r.frame.data & 0xFF, r.frame.flags);
            state = 2;
            last = millis();
        } else if(r.state == Dali::ResponseState::NO_ANSWER)
        {
            printf("Response A%i: none\n", addr);
            state = 2;
            last = millis();
        }
    } else if(state == 2)
    {
        // just delay it for 1s
        if(millis() - last > 4000)
        {
            addr++;
            if(addr > 63)
            {
                addr = 0;
            }
            state = 0;
        }
    }
}

#endif