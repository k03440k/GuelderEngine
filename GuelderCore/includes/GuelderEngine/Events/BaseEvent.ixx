//module;

//#include "../GuelderEngine.hpp"
module;
//#include "Event.hpp"
#include "../src/headers/Events/Event.hpp"
#include "../src/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Events:BaseEvent;

import GuelderEngine.Core.Types;

import <functional>;
import <array>;
import <string>;
import <sstream>;
import <type_traits>;

template<class Derived, class Base>
concept DerivedFrom = std::is_base_of_v<Base, Derived>;

export namespace GuelderEngine
{
    namespace Events
    {
//#define EVENT_STRUCT_TYPE(_type) static const EventType type = EventType::##_type;\
//                                virtual const EventType GetType() const override { return type; }\
//                                virtual const char* GetName() const override { return #_type; }
//
//#define MAKE_STRUCT_EVENT(typeEvent) struct typeEvent ## Event : public Event\
//                                     {\
//                                          typeEvent ## Event () = default;\
//                                          EVENT_STRUCT_TYPE(##typeEvent)\
//                                     }

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
        struct Event : INHERIT_GClass(Event)
        {
            virtual ~Event() = default;
            virtual const EventType GetType() const = 0;

            virtual const char* GetName() const = 0;
            virtual const std::string ToString() const { return GetName(); };

            bool isHandled = false;
        };
        class EventDispatcher : INHERIT_GClass(EventDispatcher)
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
                m_EventCallbacks[static_cast<Types::ushort>(EventType::type)] = std::move(baseCallback);
            }

            void Dispatch(Event& event)
            {
                auto& callback = m_EventCallbacks[static_cast<Types::ushort>(event.GetType())];
                if (callback)
                    callback(event);
            }

        private:
            std::array <std::function<void(Event&)>, static_cast<Types::ushort>(EventType::EventsCount)> m_EventCallbacks;
        };
        inline std::ostream& operator<<(std::ostream& os, const Event& e)//idk why inline
        {
            return os << e.ToString();
        }
    }
}