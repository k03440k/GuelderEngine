#pragma once

#include "Layer.hpp"

#include "../Utils/Utils.hpp"

#include <vector>

namespace GuelderEngine
{
    namespace Layers
    {
        class LayerStack
        {
        public:
            using LayerVec = std::vector<Layer*>;

            LayerStack();
            LayerStack(Layer* layer);
            ~LayerStack();

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* overlay);
            void PopLayer(Layer* layer);
            void PopOverlay(Layer* overlay);

            LayerVec::iterator begin() { return m_Layers.begin(); }
            LayerVec::iterator end() { return m_Layers.end(); }

            const Utils::ubyte size() const { return (Utils::ubyte)m_Layers.size(); }
            const bool IsEmpty() const { return !m_Layers.size(); }
        private:
            LayerVec m_Layers;
            LayerVec::iterator m_LayerInsert;
        };
    }
}