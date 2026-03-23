#pragma once
#define DALI_FRAME_ERROR 0b10000000
#define DALI_FRAME_COLLISION 0b00000001
#define DALI_FRAME_ECHO 0b00000001
#define DALI_FRAME_FORWARD 0b00000010
#define DALI_FRAME_BACKWARD 0b00000100

namespace Dali
{
    struct Frame
    {
        unsigned long data = 0;
        unsigned char size = 0; // in bits
        unsigned char flags = 0;
        unsigned long timestamp = 0;
        unsigned long ref = 0;
    };
}; // namespace Dali