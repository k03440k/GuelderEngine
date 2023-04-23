#pragma once

#include <string>
#include "../Events/Event.hpp"
#include "../Utils/Debug.hpp"

namespace GuelderEngine
{
    namespace Layers
    {
        class Layer
        {
        public:
            Layer(const std::string& name = "Layer") : m_DebugName(name){}
            virtual ~Layer() {}

            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate() {}
            virtual void OnEvent(const Events::Event& event) {}
            const std::string& GetName() const noexcept { return m_DebugName; }
        protected:
            std::string m_DebugName;
        };
    }
}