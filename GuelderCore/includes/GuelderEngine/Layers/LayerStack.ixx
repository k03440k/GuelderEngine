module;

//#include "Layer.hpp"

//#include "../Utils/Utils.hpp"

//#include <vector>
export module GuelderEngine.Layers:LayerStack;

import :Layer;
import GuelderEngine.Core.Types;

import <vector>;

export namespace GuelderEngine
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

            const Types::ubyte size() const { return (Types::ubyte)m_Layers.size(); }
            const bool IsEmpty() const { return !m_Layers.size(); }
        private:
            LayerVec m_Layers;
            LayerVec::iterator m_LayerInsert;
        };
    }
}