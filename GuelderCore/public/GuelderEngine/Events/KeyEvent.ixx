module;
#include "../private/headers/Events/Event.hpp"
export module GuelderEngine.Events:KeyEvent;

import :BaseEvent;

import GuelderEngine.Debug;

export namespace GuelderEngine
{
    namespace Events
    {
        using KeyCode = int;//TEMP

        struct KeyEvent : public BaseEvent
        {
        public:
            KeyCode keyCode;
        protected:
            explicit KeyEvent(const KeyCode& keyCode) : keyCode(keyCode) {}
        };
        struct KeyPressedEvent final : public KeyEvent
        {
        public:
            explicit KeyPressedEvent(const KeyCode& keyCode, const bool& isRepeat = false) :
                KeyEvent(keyCode), m_IsRepeat(isRepeat) {}

            EVENT_STRUCT_TYPE(KeyPressed)

            bool IsRepeat() const noexcept { return m_IsRepeat; }
            std::string ToString() const override
            {
                return Debug::Logger::Format("KeyPressedEvent: keyCode: ", keyCode, "(isRepeat = ", m_IsRepeat, ')');
            }
        private:
            bool m_IsRepeat : 1 = false;
        };
        struct KeyReleasedEvent final : public KeyEvent
        {
        public:
            explicit KeyReleasedEvent(const KeyCode& keyCode, const bool& isRepeat = false) :
                KeyEvent(keyCode), m_IsRepeat(isRepeat) {}

            EVENT_STRUCT_TYPE(KeyReleased)
            bool IsRepeat() const noexcept { return m_IsRepeat; }
            std::string ToString() const override
            {
                return Debug::Logger::Format("KeyPressedEvent: keyCode: ", keyCode, "(isRepeat = ", m_IsRepeat, ')');
            }
        private:
            bool m_IsRepeat : 1 = false;
        };
    }
}