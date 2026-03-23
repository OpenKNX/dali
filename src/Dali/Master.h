#pragma once
#include "Arduino.h"
#include "Commands.h"
#include "Dali/Frame.h"
#include "DataLinkLayer.h"

namespace Dali
{
    enum class ResponseState
    {
        NOT_REGISTERED,
        WAITING,
        SENT,
        RECEIVED,
        NO_ANSWER
    };

    struct Response
    {
        uint32_t sent = 0;
        uint32_t ref = 0;
        Frame frame;
        ResponseState state = ResponseState::WAITING;
    };


    class Master
    {
        DataLinkLayer _dll;
        std::vector<Response> _responses;
        void receivedFrame(Frame frame);
        uint32_t getResponseIndex(uint32_t ref);
        void removeResponse(uint32_t ref);

        public:
            void init(uint tx, uint rx);
            void process();

            void registerMonitor(std::function<void(Frame)> callback);

            /** Send an arc command to an evg/group (0-254). */
            void sendArc(uint8_t address, uint8_t value, bool isGroup = false);
            /** Send a command to an evg/group. Returns ref. */
            uint32_t sendCommand(uint8_t address, uint8_t command, bool isGroup = false, bool response = false);
            /** Send a special command to an evg/group. Returns ref. */
            uint32_t sendSpecialCommand(uint8_t command, uint8_t value = 0x00, bool response = false);
            /** Send an ExtendedCommand with a EnableDT before */
            uint32_t sendExtendedCommand(uint8_t address, uint8_t deviceType, uint8_t command, bool isGroup = false, bool response = false);
            /** Send a raw telegram */
            uint32_t sendRaw(Frame frame);

            /** Prepare a command with 16 bits. */
            uint32_t prepareCommand16(bool isGroup, uint8_t address, bool selector, uint8_t command);

            /** Get Response by ref. If it is RECEIVED or NO_ANSWER it will be automaticly deleted. */
            Response getResponse(uint32_t ref);
    };
}; // namespace Dali