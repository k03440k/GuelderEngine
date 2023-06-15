module;

//#include <string>
//#include "../Events/Event.hpp"
#include "../src/headers/Events/Event.hpp"
#include "../src/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
export module GuelderEngine.Layers:Layer;

import <string>;
import <string_view>;

export namespace GuelderEngine
{
    namespace Events { struct Event; }
    namespace Layers
    {
        class Layer : INHERIT_GClass(Layer)
        {
        public:
            Layer(const std::string& name = "Layer") : m_DebugName(name){}
            virtual ~Layer() {}

            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate() {}
            virtual void OnEvent(const Events::Event& event) {}
            std::string_view GetName() const noexcept { return m_DebugName; }
        protected:
            std::string m_DebugName;
        };
    }
}