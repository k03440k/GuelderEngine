module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
#include <compare>//wtf?!
export module GuelderEngine.Actors:World;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;
import :Actor2D;
import :Actor3D;

import <vector>;

export namespace GuelderEngine
{
    DECLARE_LOG_CATEGORY_EXTERN(World, All, true);
    class World final
    {
    public:
        World() = default;
        DECLARE_DEFAULT_MOVING(World);

        void SpawnActor(SharedPtr<Actor>& actor);
        void SpawnActor2D(SharedPtr<Actor2D>& actor);
        void SpawnActor3D(SharedPtr<Actor3D>& actor);

        void SpawnActor(SharedPtr<Actor>&& actor);
        void SpawnActor2D(SharedPtr<Actor2D>&& actor);
        void SpawnActor3D(SharedPtr<Actor3D>&& actor);

        SharedPtr<Actor> GetGeneralActor(const Actor::ID& id);
        SharedPtr<Actor>& GetActor(const Actor::ID& id = 0);
        SharedPtr<Actor2D>& GetActor2D(const Actor::ID& id = 0);
        SharedPtr<Actor3D>& GetActor3D(const Actor::ID& id = 0);

        const std::vector<WeakPtr<Actor>>& GetAllActors() const;
        const std::vector<SharedPtr<Actor>>& GetActors() const;
        const std::vector<SharedPtr<Actor2D>>& GetActors2D() const;
        const std::vector<SharedPtr<Actor3D>>& GetActors3D() const;

        void DeleteActor(const uint& id);
    private:
        friend class GEApplication;

        void CleanupActors2D(const vk::Device& device) const;
        void CleanupActors3D(const vk::Device& device) const;
        void CleanupRenderActors(const vk::Device& device) const;

        void UpdateActors();
        void BeginPlay();

        std::vector<WeakPtr<Actor>> m_AllActors;

        std::vector<SharedPtr<Actor>> m_Actors;
        std::vector<SharedPtr<Actor2D>> m_Actors2D;
        std::vector<SharedPtr<Actor3D>> m_Actors3D;
    };
}