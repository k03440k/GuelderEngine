module;
#include "../private/headers/Events/Event.hpp"
#include "../private/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
export module GuelderEngine.Layers:Layer;

import <string>;
import <string_view>;

export namespace GuelderEngine
{
    namespace Layers
    {
        class Layer// : INHERIT_GClass(Layer)
        {
        public:
            Layer(const std::string& name = "Layer") : m_DebugName(name){}
            virtual ~Layer() = default;

            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate() {}
            virtual void OnEvent(const Events::BaseEvent& event) {}
            std::string_view GetName() const noexcept { return m_DebugName; }
        protected:
            std::string m_DebugName;
        };
    }
}