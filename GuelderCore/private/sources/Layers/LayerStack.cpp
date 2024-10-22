module GuelderEngine.Layers;
import :LayerStack;

namespace GuelderEngine::Layers
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
        for(auto&& layerIt : m_Layers)
            delete layerIt;
    }
}
namespace GuelderEngine::Layers
{
    void LayerStack::PushLayer(Layer* layer)
    {
        m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
    }
    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);
    }
    void LayerStack::PopLayer(const Layer* layer)
    {
        if(const auto layerIt = std::ranges::find(m_Layers, layer); layerIt != m_Layers.end())
        {
            m_Layers.erase(layerIt);
            --m_LayerInsert;
        }
    }
    void LayerStack::PopOverlay(const Layer* overlay)
    {
        if(const auto layerIt = std::ranges::find(m_Layers, overlay); layerIt != m_Layers.end())
            m_Layers.erase(layerIt);
    }
}
