module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
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
    DECLARE_LOG_CATEGORY_EXTERN(World, All, true);
    class World final
    {
    public:
        World() = default;

        void SpawnActor(const SharedPtr<Actor>& actor);
        void SpawnActor2D(const SharedPtr<Actor2D>& actor);
        void SpawnActor3D(const SharedPtr<Actor3D>& actor);

        SharedPtr<Actor>& GetActor(const Actor::ID& id = 0);
        std::vector<SharedPtr<Actor>>& GetActors();
        SharedPtr<Actor2D>& GetActor2D(const Actor::ID& id = 0);
        SharedPtr<Actor3D>& GetActor3D(const Actor::ID& id = 0);
        std::vector<SharedPtr<Actor2D>>& GetActors2D();
        std::vector<SharedPtr<Actor3D>>& GetActors3D();

        void CleanupActors2D(const vk::Device& device) const;
        void CleanupActors3D(const vk::Device& device) const;
        void CleanupRenderActors(const vk::Device& device) const;

        //const std::vector<Vulkan::Buffers::VertexBuffer>& GetVertexBuffers() const noexcept;
    private:
        friend class GEApplication;
        void UpdateActors();
        void Begin();

        std::vector<SharedPtr<Actor>> m_Actors;
        std::vector<SharedPtr<Actor2D>> m_Actors2D;
        std::vector<SharedPtr<Actor3D>> m_Actors3D;
        //std::vector<Vulkan::Buffers::VertexBuffer> m_VertexBuffers;
    };
}