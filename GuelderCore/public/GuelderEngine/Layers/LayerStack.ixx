export module GuelderEngine.Layers:LayerStack;

import :Layer;
import GuelderEngine.Core.Types;

import <vector>;

export namespace GuelderEngine::Layers
{
    class LayerStack
    {
    public:
        using LayerVec = std::vector<Layer*>;

        LayerStack();
        explicit LayerStack(Layer* layer);
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(const Layer* layer);
        void PopOverlay(const Layer* overlay);

        LayerVec::iterator begin() { return m_Layers.begin(); }
        LayerVec::iterator end() { return m_Layers.end(); }

        ubyte size() const { return (ubyte)m_Layers.size(); }
        bool IsEmpty() const { return !m_Layers.size(); }
    private:
        LayerVec m_Layers;
        LayerVec::iterator m_LayerInsert;
    };
}
