module;
//#include "Event.hpp"
#include "../private/headers/Events/Event.hpp"
#include "../Utils/Debug.hpp"
export module GuelderEngine.Events:MouseEvents;

import :BaseEvent;

import GuelderEngine.Debug;

export namespace GuelderEngine
{
    namespace Events
    {
        using MouseCode = int;//TEMP
        struct MouseMovedEvent : public BaseEvent
        {
            MouseMovedEvent(const double& newX, const double& newY) : x(newX), y(newY) {};

            double x;
            double y;

            EVENT_STRUCT_TYPE(MouseMoved)
            std::string ToString() const override
            {
                return Debug::Logger::Format("MouseMovedEvent: x[", x, "], y[", y, ']');
            }
        };
        struct MouseScrolledEvent : public BaseEvent
        {
            MouseScrolledEvent(const float& xOffset, const float& yOffset) : xOffset(xOffset), yOffset(yOffset) {};

            float xOffset;
            float yOffset;

            EVENT_STRUCT_TYPE(MouseScrolled)
            std::string ToString() const override
            {
                return Debug::Logger::Format("MouseScrolledEvent: x[", xOffset, "], y[", yOffset, ']');
            }
        };
        struct MouseButtonEvent : public BaseEvent
        {
        public:
            MouseCode button;
        protected:
            MouseButtonEvent(const MouseCode button) : button(button){}
            EVENT_STRUCT_TYPE(MouseButtonPressed)
        };
        struct MouseButtonPressedEvent : public MouseButtonEvent
        {
            MouseButtonPressedEvent(const MouseCode& button) : MouseButtonEvent(button) {}

            EVENT_STRUCT_TYPE(MouseButtonPressed)//idk which
            std::string ToString() const override
            {
                return Debug::Logger::Format("MouseButtonPressedEvent: button: ", button);
            }
        };
        struct MouseButtonReleasedEvent : public MouseButtonEvent
        {
            MouseButtonReleasedEvent(const MouseCode& button) : MouseButtonEvent(button) {}

            EVENT_STRUCT_TYPE(MouseButtonReleased)//idk which
            std::string ToString() const override
            {
                return Debug::Logger::Format("MouseButtonReleasedEvent: button: ", button);
            }
        };
    }
}