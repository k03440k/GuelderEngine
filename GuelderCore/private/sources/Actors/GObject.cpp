module;
module GuelderEngine.Actors;
import :GObject;

namespace GuelderEngine
{
    GObject::ID GObject::currentID = 0;

    GObject::GObject()
    {
        m_ID = currentID++;
    }
    GObject::GObject(const GObject& other)
    {
        m_ID = currentID++;
    }
    GObject& GObject::operator=(const GObject& other)
    {
        m_ID = currentID++;

        return *this;
    }

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