module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
#include <compare>//wtf?!
export module GuelderEngine.Game:World;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;
import :Object2D;

import <vector>;

export namespace GuelderEngine
{
    class World final
    {
    public:
        World() = default;

        void SpawnActor(Actor&& actor);
        void SpawnRenderActor(Object2DPtr actor);

        ActorPtr& GetActor(const Actor::ID& id = 0);
        std::vector<ActorPtr>& GetActors();
        Object2DPtr& GetRenderActor(const Actor::ID& id = 0);
        std::vector<Object2DPtr>& GetRenderActors();

        //const std::vector<Vulkan::Buffers::VertexBuffer>& GetVertexBuffers() const noexcept;
    private:
        friend class GEApplication;
        void UpdateActors();
        void Begin();

        std::vector<ActorPtr> m_Actors;
        std::vector<Object2DPtr> m_RenderActors;
        //std::vector<Vulkan::Buffers::VertexBuffer> m_VertexBuffers;
    };
}