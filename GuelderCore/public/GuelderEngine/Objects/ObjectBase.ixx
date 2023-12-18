module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Game:GObjectBase;

export namespace GuelderEngine
{
    class GObjectBase
    {
    public:
        using ID = size_t;
    public:
        GObjectBase();
        virtual ~GObjectBase() = default;

        DECLARE_COPY_AND_MOVING(GObjectBase);

        ID GetID() const noexcept { return m_ID; }

    protected:
        static ID currentID;

        ID m_ID;
    };
}