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
    void World::UpdateActors()
    {
        std::thread renderActors([&] {std::for_each(std::execution::par, m_RenderActors.begin(), m_RenderActors.end(), [](SharedPtr<RenderActor> actor) { actor->Update(); }); });
        if(!m_Actors.empty())
        {
            std::thread actors([&] {std::for_each(std::execution::par, m_Actors.begin(), m_Actors.end(), [](SharedPtr<Actor> actor) { actor->Update(); }); });
            actors.join();
        }
        renderActors.join();
    }
    void World::Begin()
    {
        std::ranges::for_each(m_Actors, [](SharedPtr<Actor> actor) {actor->Begin(); });
    }
    void World::SpawnActor(SharedPtr<Actor> actor)
    {
        m_Actors.push_back(actor);
    }
    void World::SpawnRenderActor(SharedPtr<RenderActor> actor)
    {
        if(!actor->IsComplete())
            return;
        
        //actor->Update();
        m_RenderActors.push_back(actor);
    }
    SharedPtr<Actor>& World::GetActor(const Actor::ID& id)
    {
        return m_Actors[id];
    }
    std::vector<SharedPtr<Actor>>& World::GetActors()
    {
        return m_Actors;
    }
    SharedPtr<RenderActor>& World::GetRenderActor(const Actor::ID& id)
    {
        return m_RenderActors[id];
    }
    std::vector<SharedPtr<RenderActor>>& World::GetRenderActors()
    {
        return m_RenderActors;
    }
    /*const std::vector<Vulkan::Buffers::VertexBuffer>& World::GetVertexBuffers() const noexcept
    {
        return m_VertexBuffers;
    }*/
}