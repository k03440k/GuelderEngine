#pragma once

#include "Event.hpp"

namespace GuelderEngine
{
    namespace Events
    {
        struct WindowResizeEvent : public Event
        {
            WindowResizeEvent(const Utils::ushort& newWidth, const Utils::ushort& newHeight) :
                width(newWidth), height(newHeight) {}

            Utils::ushort width;
            Utils::ushort height;

            EVENT_STRUCT_TYPE(WindowResize)
            const std::string ToString() const override
            {
                //std::stringstream ss;
                //ss << "WindowResizeEvent: width[" << width << "], height[" << height << ']';
                
                return Logger::Format("WindowResizeEvent: width[", width, "], height[", height, ']');
            }
        };
        MAKE_STRUCT_EVENT(WindowClose);
        MAKE_STRUCT_EVENT(AppUpdate);
        MAKE_STRUCT_EVENT(AppRender);
    }
}