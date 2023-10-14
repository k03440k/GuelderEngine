module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
#include <compare>//wtf?!
module GuelderEngine.Game;
import :World;

import GuelderEngine.Vulkan;
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
        std::thread renderActors([&] {std::for_each(std::execution::par, m_RenderActors.begin(), m_RenderActors.end(), [](RenderActorPtr actor) { actor->Update(); }); });
        if(!m_Actors.empty())
        {
            std::thread actors([&] {std::for_each(std::execution::par, m_Actors.begin(), m_Actors.end(), [](ActorPtr actor) { actor->Update(); }); });
            actors.join();
        }
        renderActors.join();
    }
    void World::Begin()
    {
        std::ranges::for_each(m_Actors, [](ActorPtr& actor) {actor->Begin(); });
    }
    void World::SpawnActor(Actor&& actor)
    {
        m_Actors.push_back(MakeActor<Actor>(std::forward<Actor>(actor)));
    }
    void World::SpawnRenderActor(Object2DPtr actor)
    {
        /*if(!actor.IsComplete())
            return;*/

        //auto a = std::make_shared<Object2D>(std::forward<Object2D>(actor));
        actor->Update();
        m_RenderActors.push_back(actor);
        //m_VertexBuffers.push_back(actor.vertexBuffer);
    }
    ActorPtr& World::GetActor(const Actor::ID& id)
    {
        return m_Actors[id];
    }
    std::vector<ActorPtr>& World::GetActors()
    {
        return m_Actors;
    }
    Object2DPtr& World::GetRenderActor(const Actor::ID& id)
    {
        return m_RenderActors[id];
    }
    std::vector<Object2DPtr>& World::GetRenderActors()
    {
        return m_RenderActors;
    }
    /*const std::vector<Vulkan::Buffers::VertexBuffer>& World::GetVertexBuffers() const noexcept
    {
        return m_VertexBuffers;
    }*/
}