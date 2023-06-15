//module;

//#include "Event.hpp"
module;
#include "../src/headers/Events/Event.hpp"
#include "../src/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Events:KeyEvent;

import :BaseEvent;

import GuelderEngine.Debug;

export namespace GuelderEngine
{
    namespace Events
    {
        using KeyCode = int;//TEMP

        struct KeyEvent : public Event
        {
        public:
            KeyCode keyCode;
        protected:
            KeyEvent(const KeyCode& keyCode) : keyCode(keyCode) {}
        };
        struct KeyPressedEvent : public KeyEvent
        {
        public:
            KeyPressedEvent(const KeyCode& keyCode, const bool& isRepeat = false) :
                KeyEvent(keyCode), m_IsRepeat(isRepeat) {}

            EVENT_STRUCT_TYPE(KeyPressed)

            const bool IsRepeat() { return m_IsRepeat; }
            const std::string ToString() const override
            {
                //std::stringstream ss;
                //ss << "KeyPressedEvent: keyCode: " << keyCode << "(isRepeat = " << isRepeat << ')';
                return Debug::Logger::Format("KeyPressedEvent: keyCode: ", keyCode, "(isRepeat = ", m_IsRepeat, ')');
            }
        private:
            bool m_IsRepeat = false;
        };
        struct KeyReleasedEvent : public KeyEvent
        {
        public:
            KeyReleasedEvent(const KeyCode& keyCode, const bool& isRepeat = false) :
                KeyEvent(keyCode), m_IsRepeat(isRepeat) {}

            EVENT_STRUCT_TYPE(KeyReleased)
            const bool IsRepeat() { return m_IsRepeat; }
            const std::string ToString() const override
            {
                //std::stringstream ss;
                //ss << "KeyReleasedEvent: button: " << keyCode << "(isRepeat = " << isRepeat << ')';
                return Debug::Logger::Format("KeyPressedEvent: keyCode: ", keyCode, "(isRepeat = ", m_IsRepeat, ')');
            }
        private:
            bool m_IsRepeat = false;
        };
    }
}