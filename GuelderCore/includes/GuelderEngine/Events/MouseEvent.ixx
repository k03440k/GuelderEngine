module;
//#include "Event.hpp"
#include "../src/headers/Events/Event.hpp"
#include "../src/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
export module GuelderEngine.Events:MouseEvents;

import :BaseEvent;

import GuelderEngine.Debug;

export namespace GuelderEngine
{
    namespace Events
    {
        using MouseCode = int;//TEMP
        struct MouseMovedEvent : public Event
        {
            MouseMovedEvent(const double& newX, const double& newY) : x(newX), y(newY) {};

            double x;
            double y;

            EVENT_STRUCT_TYPE(MouseMoved)
            const std::string ToString() const override
            {
                //std::stringstream ss;
                //ss << "MouseMovedEvent: x[" << x << "], y[" << y << ']';
                return Debug::Logger::Format("MouseMovedEvent: x[", x, "], y[", y, ']');
            }
        };
        struct MouseScrolledEvent : public Event
        {
            MouseScrolledEvent(const float& xOffset, const float& yOffset) : xOffset(xOffset), yOffset(yOffset) {};

            float xOffset;
            float yOffset;

            EVENT_STRUCT_TYPE(MouseScrolled)
            const std::string ToString() const override
            {
                //std::stringstream ss;
                //ss << "MouseScrolledEvent: xOffset[" << xOffset << "], yOffset[" << yOffset << ']';
                return Debug::Logger::Format("MouseScrolledEvent: x[", xOffset, "], y[", yOffset, ']');
            }
        };
        struct MouseButtonEvent : public Event
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
            const std::string ToString() const override
            {
                //std::stringstream ss;
                //ss << "MouseButtonPressedEvent: button: " << button;
                //return ss.str();
                return Debug::Logger::Format("MouseButtonPressedEvent: button: ", button);
            }
        };
        struct MouseButtonReleasedEvent : public MouseButtonEvent
        {
            MouseButtonReleasedEvent(const MouseCode& button) : MouseButtonEvent(button) {}

            EVENT_STRUCT_TYPE(MouseButtonReleased)//idk which
            const std::string ToString() const override
            {
                //std::stringstream ss;
                //ss << "MouseButtonReleasedEvent: button: " << button;
                //return ss.str();
                return Debug::Logger::Format("MouseButtonReleasedEvent: button: ", button);
            }
        };
    }
}