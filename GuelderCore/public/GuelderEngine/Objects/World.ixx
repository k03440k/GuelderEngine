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

        void SpawnActor(SharedPtr<Actor> actor);
        void SpawnRenderActor(SharedPtr<RenderActor> actor);

        SharedPtr<Actor>& GetActor(const Actor::ID& id = 0);
        std::vector<SharedPtr<Actor>>& GetActors();
        SharedPtr<RenderActor>& GetRenderActor(const Actor::ID& id = 0);
        std::vector<SharedPtr<RenderActor>>& GetRenderActors();

        //const std::vector<Vulkan::Buffers::VertexBuffer>& GetVertexBuffers() const noexcept;
    private:
        friend class GEApplication;
        void UpdateActors();
        void Begin();

        std::vector<SharedPtr<Actor>> m_Actors;
        std::vector<SharedPtr<RenderActor>> m_RenderActors;
        //std::vector<Vulkan::Buffers::VertexBuffer> m_VertexBuffers;
    };
}