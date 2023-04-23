#pragma once

#include "../Utils/Utils.hpp"
#include "../Utils/Debug.hpp"

#include <functional>
#include <array>
#include <string>
#include <sstream>
#include <type_traits>

template<class Derived, class Base>
concept DerivedFrom = std::is_base_of_v<Base, Derived>;

namespace GuelderEngine
{
    namespace Events
    {
#define EVENT_STRUCT_TYPE(_type) static const EventType type = EventType::##_type;\
                                virtual const EventType GetType() const override { return type; }\
                                virtual const char* GetName() const override { return #_type; }

#define MAKE_STRUCT_EVENT(typeEvent) struct typeEvent ## Event : public Event\
                                     {\
                                          typeEvent ## Event () = default;\
                                          EVENT_STRUCT_TYPE(##typeEvent)\
                                     }

        enum class EventType
        {
            WindowResize,
            WindowClose,

            //KeyJustPressed,
            KeyPressed,
            KeyReleased,

            //MouseButtonJustPressed,
            MouseButtonPressed,
            MouseButtonReleased,
            MouseScrolled,
            MouseMoved,

            AppUpdate,
            AppRender,

            EventsCount
        };
        struct Event
        {
            virtual ~Event() = default;
            virtual const EventType GetType() const = 0;

            virtual const char* GetName() const = 0;
            virtual const std::string ToString() const { return GetName(); };

            bool isHandled = false;
        };
        class EventDispatcher
        {
        public:
            /*
            * template class should be inheriented from struct Event
            */
            template<DerivedFrom<Event> EventType = Event>
            void AddEventListener(std::function<void(EventType&)> callback)
            {
                auto baseCallback = [func = std::move(callback)](Event& e)
                {
                    if (e.GetType() == EventType::type) func(static_cast<EventType&>(e));
                };
                m_EventCallbacks[static_cast<Utils::ushort>(EventType::type)] = std::move(baseCallback);
            }

            void Dispatch(Event& event)
            {
                auto& callback = m_EventCallbacks[static_cast<Utils::ushort>(event.GetType())];
                if (callback)
                    callback(event);
            }

        private:
            std::array <std::function<void(Event&)>, static_cast<Utils::ushort>(EventType::EventsCount)> m_EventCallbacks;
        };
        inline std::ostream& operator<<(std::ostream& os, const Event& e)//idk why inline
        {
            return os << e.ToString();
        }
    }
}