module;
module GuelderEngine.Game;
import :GObjectBase;

namespace GuelderEngine
{
    GObjectBase::ID GObjectBase::currentID = 0;

    GObjectBase::GObjectBase()
    {
        m_ID = currentID++;
    }
    GObjectBase::GObjectBase(const GObjectBase& other)
    {
        m_ID = currentID++;
    }
    GObjectBase::GObjectBase(GObjectBase&& other) noexcept
    {
        m_ID = other.m_ID;
    }
    GObjectBase& GObjectBase::operator=(const GObjectBase& other)
    {
        m_ID = currentID++;

        return *this;
    }
    GObjectBase& GObjectBase::operator=(GObjectBase&& other) noexcept
    {
        m_ID = other.m_ID;

        return *this;
    }
}