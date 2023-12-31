module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
#include <compare>//wtf?!
module GuelderEngine.Game;
import :World;

import GuelderEngine.Vulkan;
import GuelderEngine.Core.Types;
import :Actor;

import <algorithm>;
import <execution>;
import <ranges>;
import <thread>;
import <vector>;

namespace GuelderEngine
{
    DEFINE_LOG_CATEGORY(World);
    void World::UpdateActors()
    {
        std::thread renderActors3D([&] {std::for_each(std::execution::par, m_Actors2D.begin(), m_Actors2D.end(), [](const SharedPtr<Actor2D>& actor) { actor->Update(); }); });
        std::thread renderActors2D([&] {std::for_each(std::execution::par, m_Actors3D.begin(), m_Actors3D.end(), [](const SharedPtr<Actor3D>& actor) { actor->Update(); }); });
        if(!m_Actors.empty())
        {
            std::thread actors([&] {std::for_each(std::execution::par, m_Actors.begin(), m_Actors.end(), [](const SharedPtr<Actor>& actor) { actor->Update(); }); });
            actors.join();
        }
        renderActors2D.join();
        renderActors3D.join();
    }
    void World::Begin()
    {
        std::ranges::for_each(m_Actors, [](const SharedPtr<Actor>& actor) {actor->Begin(); });
        std::ranges::for_each(m_Actors2D, [](const SharedPtr<Actor2D>& actor) {actor->Begin(); });
        std::ranges::for_each(m_Actors3D, [](const SharedPtr<Actor3D>& actor) {actor->Begin(); });
    }
    void World::SpawnActor(const SharedPtr<Actor>& actor)
    {
        m_Actors.push_back(actor);
    }
    void World::SpawnActor2D(const SharedPtr<Actor2D>& actor)
    {
        if(!actor->IsComplete())
        {
            GE_LOG(World, Warning, "the actor with id ", actor->GetID(), " is incomplete and it won't be spawn");
            return;
        }
        
        //actor->Update();
        m_Actors2D.push_back(actor);
    }
    void World::SpawnActor3D(const SharedPtr<Actor3D>& actor)
    {
        if(!actor->IsComplete())
        {
            GE_LOG(World, Warning, "the actor with id ", actor->GetID(), " is incomplete and it won't be spawn");
            return;
        }

        m_Actors3D.push_back(actor);
    }
    SharedPtr<Actor>& World::GetActor(const Actor::ID& id)
    {
        return m_Actors[id];
    }
    std::vector<SharedPtr<Actor>>& World::GetActors()
    {
        return m_Actors;
    }
    SharedPtr<Actor2D>& World::GetActor2D(const Actor::ID& id)
    {
        return m_Actors2D[id];
    }
    SharedPtr<Actor3D>& World::GetActor3D(const Actor::ID& id)
    {
        return m_Actors3D[id];
    }
    std::vector<SharedPtr<Actor2D>>& World::GetActors2D()
    {
        return m_Actors2D;
    }
    std::vector<SharedPtr<Actor3D>>& World::GetActors3D()
    {
        return m_Actors3D;
    }
    void World::CleanupActors2D(const vk::Device& device) const
    {
        std::ranges::for_each(m_Actors2D, [&device](const SharedPtr<Actor2D>& actor) { actor->Cleanup(device); });
    }
    void World::CleanupActors3D(const vk::Device& device) const
    {
        std::ranges::for_each(m_Actors3D, [&device](const SharedPtr<Actor3D>& actor) { actor->Cleanup(device); });
    }
    void World::CleanupRenderActors(const vk::Device& device) const
    {
        CleanupActors2D(device);
        CleanupActors3D(device);
    }
    /*const std::vector<Vulkan::Buffers::VertexBuffer>& World::GetVertexBuffers() const noexcept
    {
        return m_VertexBuffers;
    }*/
}