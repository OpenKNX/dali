#include "Master.h"

namespace Dali
{
    void Master::init(uint tx, uint rx)
    {
        _dll.init(tx, rx);
        _dll.registerMonitor([this](Frame frame) { this->receivedFrame(frame); });
    }

    void Master::process()
    {
        _dll.process();

        for(auto &r : _responses)
        {
            if(r.state == ResponseState::SENT && ((micros() - r.sent) > 50000))
            {
                r.state = ResponseState::NO_ANSWER;
            }

            if((r.state == ResponseState::NO_ANSWER || r.state == ResponseState::RECEIVED)
                 && ((micros() - r.sent) > 100000))
            {
                // We got a response but no one cares...
                // printf("Response %u not handled\n", r.ref);
                removeResponse(r.ref);
            }

            if(r.state == ResponseState::WAITING && ((micros() - r.ref) > 10000000))
            {
                // It seems we did not send it...
                // TODO handle it
                printf("Frame not sent %i - %i (%i)\n", r.ref, micros(), micros() - r.ref);
                removeResponse(r.ref);
            }
        }
    }

    void Master::removeResponse(uint32_t ref)
    {
        uint32_t index = getResponseIndex(ref);
        if(index != -1)
        {
            _responses.erase(_responses.begin() + index);
        }
    }

    uint32_t Master::getResponseIndex(uint32_t ref)
    {
        for(size_t i = 0; i < _responses.size(); i++)
        {
            if(_responses[i].ref == ref)
            {
                return i;
            }
        }

        return -1;
    }

    Response Master::getResponse(uint32_t ref)
    {
        for(auto &r : _responses)
        {
            if(r.ref == ref)
            {
                if(r.state == ResponseState::RECEIVED || r.state == ResponseState::NO_ANSWER)
                    removeResponse(ref);
                return r;
            }
        }

        Response r;
        r.state = ResponseState::NOT_REGISTERED;
        return r;
    }

    void Master::registerMonitor(std::function<void(Frame)> callback)
    {
        _dll.registerMonitor(callback);
    }

    void Master::receivedFrame(Frame frame)
    {
        for(Response &r : _responses)
        {
            if(r.ref == frame.ref)
            {
                if(frame.flags & DALI_FRAME_BACKWARD)
                {
                    r.frame = frame;
                    r.state = ResponseState::RECEIVED;
                }
                else
                {
                    r.state = ResponseState::SENT;
                    r.sent = frame.timestamp;
                }
            }
        }
    }

    void Master::sendArc(uint8_t address, uint8_t value, bool isGroup)
    {
        Frame frame;
        frame.data = prepareCommand16(isGroup, address, false, value);
        frame.flags = DALI_FRAME_FORWARD;
        frame.size = 16;
        _dll.transmitFrame(frame);
    }

    uint32_t Master::sendCommand(uint8_t address, uint8_t command, bool isGroup, bool response)
    {
        Frame frame;
        frame.data = prepareCommand16(isGroup, address, true, command);
        frame.flags = DALI_FRAME_FORWARD;
        frame.size = 16;
        frame.ref = micros();

        if(command >= 32 && command <= 129)
        {
            // This is a configuration command so send it twice
            _dll.transmitFrame(frame);
            frame.ref += 1;
        }

        if(response)
        {
            Response r;
            r.ref = frame.ref;
            _responses.push_back(r);
        }

        _dll.transmitFrame(frame);
        return frame.ref;
    }

    uint32_t Master::sendSpecialCommand(uint8_t command, uint8_t value, bool response)
    {
        Frame frame;
        frame.data = prepareCommand16(true, command, true, value);
        frame.flags = DALI_FRAME_FORWARD;
        frame.size = 16;
        frame.ref = micros();
        
        if(command == SpecialCommand::INITIALISE || command == SpecialCommand::RANDOMISE)
        {
            // This is a configuration command so send it twice
            _dll.transmitFrame(frame);
            frame.ref += 1;
        }

        if(response)
        {
            Response r;
            r.ref = frame.ref;
            _responses.push_back(r);
        }

        _dll.transmitFrame(frame);
        return frame.ref;
    }

    uint32_t Master::sendExtendedCommand(uint8_t address, uint8_t deviceType, uint8_t command, bool isGroup, bool response)
    {
        sendSpecialCommand(SpecialCommand::ENABLE_DT, deviceType, false);
        return sendCommand(address, command, isGroup, response);
    }

    uint32_t Master::sendRaw(Frame frame)
    {
        frame.ref = micros();
        _dll.transmitFrame(frame);
        return frame.ref;
    }

    uint32_t Master::prepareCommand16(bool isGroup, uint8_t byte0, bool selector, uint8_t byte1)
    {
        uint32_t data = 0;
        data |= (isGroup ? 1 : 0) << 15;
        data |= (byte0 & 0x3F) << 9;
        data |= (selector ? 1 : 0) << 8;
        data |= (byte1 & 0xFF);
        return data;
    }
};