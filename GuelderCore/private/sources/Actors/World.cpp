module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
#include <compare>//wtf?!
module GuelderEngine.Actors;
import :World;

import GuelderEngine.Vulkan;
import GuelderEngine.Core.Types;
import :GObject;
import :Actor;
import :Actor2D;
import :Actor3D;

import <algorithm>;
import <execution>;
import <ranges>;
import <thread>;
import <vector>;
import <memory>;

namespace GuelderEngine
{
    DEFINE_LOG_CATEGORY(World);
    void World::UpdateActors()
    {
        std::for_each(std::execution::par, m_AllActors.begin(), m_AllActors.end(), []
        (WeakPtr<Actor>& actor)
            {
                actor.lock()->Update();

            }
        );
    }
    void World::BeginPlay()
    {
        std::for_each(std::execution::par, m_AllActors.begin(), m_AllActors.end(), []
        (WeakPtr<Actor>& actor)
            {
                actor.lock()->BeginPlay();
            }
        );
    }
    void World::SpawnActor(SharedPtr<Actor>& actor)
    {
        actor->m_ID = GObject::currentID++;
        m_Actors.push_back(actor);
        m_AllActors.push_back(actor);
    }
    void World::SpawnActor2D(SharedPtr<Actor2D>& actor)
    {
        actor->m_ID = GObject::currentID++;
        if(!actor->IsComplete())
        {
            GE_LOG(World, Warning, "the actor with id ", actor->GetID(), " is incomplete");
            //return;
        }

        m_Actors2D.push_back(actor);
        m_AllActors.push_back(actor);
    }
    void World::SpawnActor3D(SharedPtr<Actor3D>& actor)
    {
        actor->m_ID = GObject::currentID++;
        if(!actor->IsComplete())
        {
            GE_LOG(World, Warning, "the actor with id ", actor->GetID(), " is incomplete");
            //return;
        }

        m_Actors3D.push_back(actor);
        m_AllActors.push_back(actor);
    }
    void World::SpawnActor(SharedPtr<Actor>&& actor)
    {
        actor->m_ID = GObject::currentID++;
        m_Actors.push_back(std::move(actor));
        m_AllActors.push_back(m_Actors[m_Actors.size()-1]);
    }
    void World::SpawnActor2D(SharedPtr<Actor2D>&& actor)
    {
        actor->m_ID = GObject::currentID++;
        if(!actor->IsComplete())
        {
            GE_LOG(World, Warning, "the actor with id ", actor->GetID(), " is incomplete");
            //return;
        }

        m_Actors2D.push_back(std::move(actor));
        m_AllActors.push_back(m_Actors2D[m_Actors2D.size()-1]);
    }
    void World::SpawnActor3D(SharedPtr<Actor3D>&& actor)
    {
        actor->m_ID = GObject::currentID++;
        if(!actor->IsComplete())
        {
            GE_LOG(World, Warning, "the actor with id ", actor->GetID(), " is incomplete");
            //return;
        }

        m_Actors3D.push_back(std::move(actor));
        m_AllActors.push_back(m_Actors3D[m_Actors3D.size()-1]);
    }
    //TODO: make Getting actors via real ID(std::find_if()) and finish fucking ID system
    SharedPtr<Actor> World::GetGeneralActor(const Actor::ID& id)
    {
        return std::ranges::find_if(m_AllActors, [&id]
                                (const WeakPtr<Actor>& actor)
                                    {
                                        return actor.lock()->GetID() == id;
                                    }
        )->lock();
    }
    SharedPtr<Actor>& World::GetActor(const Actor::ID& id)
    {
        return *std::ranges::find_if(m_Actors, [&id]
        (const SharedPtr<Actor>& actor)
            {
                return actor->GetID() == id;
            }
        );
    }
    SharedPtr<Actor2D>& World::GetActor2D(const Actor::ID& id)
    {
        return *std::ranges::find_if(m_Actors2D, [&id]
        (const SharedPtr<Actor2D>& actor)
            {
                return actor->GetID() == id;
            }
        );
    }
    SharedPtr<Actor3D>& World::GetActor3D(const Actor::ID& id)
    {
        return *std::ranges::find_if(m_Actors3D, [&id]
        (const SharedPtr<Actor3D>& actor)
            {
                return actor->GetID() == id;
            }
        );
    }
    const std::vector<WeakPtr<Actor>>& World::GetAllActors() const
    {
        return m_AllActors;
    }
    const std::vector<SharedPtr<Actor>>& World::GetActors() const
    {
        return m_Actors;
    }
    const std::vector<SharedPtr<Actor2D>>& World::GetActors2D() const
    {
        return m_Actors2D;
    }
    const std::vector<SharedPtr<Actor3D>>& World::GetActors3D() const
    {
        return m_Actors3D;
    }
    void World::DeleteActor(const uint& id)
    {
        /*auto found = std::ranges::find_if(m_AllActors, []
        (const std::shared_ptr<Actor>& actor)
        {
                if(actor->GetID() == id)
                    return actor;
        }
        );

        auto found2D = std::ranges::find_if(m_Actors2D, []
        (const std::shared_ptr<Actor2D>& actor)
            {
                if(actor->GetID() == id)
                    return actor;
            }
        );
        auto found3D = std::ranges::find_if(m_Actors3D, []
        (const std::shared_ptr<Actor3D>& actor)
            {
                if(actor->GetID() == id)
                    return actor;
            }
        );
        m_AllActors.erase(found);
        m_Actors2D.erase(found2D);
        m_Actors3D.erase(found3D);*/
    }
    void World::CleanupActors2D(const vk::Device& device) const
    {
        std::ranges::for_each(m_Actors2D, [&device](const SharedPtr<Actor2D>& actor) { actor->Cleanup(device); });
    }
    void World::CleanupActors3D(const vk::Device& device) const
    {
        std::ranges::for_each(m_Actors3D, [&device](const SharedPtr<Actor3D>& actor) { if(actor->IsComplete())actor->Cleanup(device); });
    }
    void World::CleanupRenderActors(const vk::Device& device) const
    {
        CleanupActors2D(device);
        CleanupActors3D(device);
    }
}