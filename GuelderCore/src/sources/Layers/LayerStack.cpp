#include "../includes/GuelderEngine/Layers/LayerStack.hpp"

namespace GuelderEngine
{
    namespace Layers
    {
        LayerStack::LayerStack()
        {
            m_LayerInsert = m_Layers.begin();
        }
        LayerStack::LayerStack(Layer* layer)
        {
            m_LayerInsert = m_Layers.begin();
            PushLayer(layer);
        }
        LayerStack::~LayerStack()
        {
            for (auto layer_it : m_Layers)
                delete layer_it;
        }
        void LayerStack::PushLayer(Layer* layer)
        {
            m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
        }
        void LayerStack::PushOverlay(Layer* overlay)
        {
            m_Layers.emplace_back(overlay);
        }
        void LayerStack::PopLayer(Layer* layer)
        {
            auto layer_it = std::find(m_Layers.begin(), m_Layers.end(), layer);
            if (layer_it != m_Layers.end())
            {
                m_Layers.erase(layer_it);
                m_LayerInsert--;
            }
        }
        void LayerStack::PopOverlay(Layer* overlay)
        {
            auto layer_it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
            if (layer_it != m_Layers.end())
                m_Layers.erase(layer_it);
        }
    }
}