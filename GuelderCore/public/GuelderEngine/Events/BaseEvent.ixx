module;
#include "../private/headers/Core/GObject/GClass.hpp"
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
        struct BaseEvent
        {
            virtual ~BaseEvent() = default;
            virtual const EventType GetType() const = 0;

            virtual const char* GetName() const = 0;
            virtual std::string ToString() const { return GetName(); };

            bool isHandled : 1 = false;
        };
        class EventDispatcher// : INHERIT_GClass(EventDispatcher)
        {
        public:
            /*
            * template class should be inherited from struct BaseEvent
            */
            template<DerivedFrom<BaseEvent> EventType = BaseEvent>
            void AddEventListener(std::function<void(EventType&)> callback)
            {
                auto baseCallback = [func = std::move(callback)](BaseEvent& e)
                {
                    if (e.GetType() == EventType::type) func(static_cast<EventType&>(e));
                };
                m_EventCallbacks[static_cast<ushort>(EventType::type)] = std::move(baseCallback);
            }

            void Dispatch(BaseEvent& event) const
            {
                if (auto& callback = m_EventCallbacks[static_cast<ushort>(event.GetType())])
                    callback(event);
            }

        private:
            std::array <std::function<void(BaseEvent&)>, static_cast<ushort>(EventType::EventsCount)> m_EventCallbacks;
        };
        inline std::ostream& operator<<(std::ostream& os, const BaseEvent& e)//idk why inline
        {
            return os << e.ToString();
        }
    }
}