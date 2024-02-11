module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Actors;
import :GObject;

namespace GuelderEngine
{
    GObject::ID GObject::currentID = 0;

    GObjectComponent::ID GObjectComponent::currentID = 0;

    GObjectComponent::GObjectComponent()
    {
        m_ID = currentID++;
    }
    GObjectComponent::GObjectComponent(const GObjectComponent& other)
    {
        m_ID = currentID++;
    }
    GObjectComponent& GObjectComponent::operator=(const GObjectComponent& other)
    {
        m_ID = currentID++;

        return *this;
    }
}