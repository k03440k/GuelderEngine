module;
#include "../private/headers/Events/Event.hpp"
#include "../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Events:ApplicationEvents;

import :BaseEvent;

import GuelderEngine.Debug;

export namespace GuelderEngine
{
    namespace Events
    {
        struct WindowResizeEvent : public BaseEvent
        {
            WindowResizeEvent(const Types::ushort& newWidth, const Types::ushort& newHeight) :
                width(newWidth), height(newHeight) {}

            Types::ushort width;
            Types::ushort height;

            EVENT_STRUCT_TYPE(WindowResize)
            std::string ToString() const override
            {
                //std::stringstream ss;
                //ss << "WindowResizeEvent: width[" << width << "], height[" << height << ']';
                
                return Debug::Logger::Format("WindowResizeEvent: width[", width, "], height[", height, ']');
            }
        };
        MAKE_STRUCT_EVENT(WindowClose);
        MAKE_STRUCT_EVENT(AppUpdate);
        MAKE_STRUCT_EVENT(AppRender);
    }
}