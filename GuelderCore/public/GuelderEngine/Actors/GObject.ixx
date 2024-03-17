module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Actors:GObject;

export namespace GuelderEngine
{
    class GObject
    {
    private:
        friend class World;
    public:
        using ID = size_t;
    public:
        GObject() = default;
        virtual ~GObject() = default;

        DECLARE_DEFAULT_COPYING_AND_MOVING(GObject);
        //DECLARE_DEFAULT_MOVING(GObject);

        ID GetID() const noexcept { return m_ID; }

    private:
        static ID currentID;

        ID m_ID;
    };
    //TODO: do something with ids in components(probably delete)
    class GObjectComponent
    {
    public:
        using ID = size_t;
    public:
        GObjectComponent();
        virtual ~GObjectComponent() = default;

        DECLARE_COPYING(GObjectComponent);
        DECLARE_DEFAULT_MOVING(GObjectComponent);

        ID GetID() const noexcept { return m_ID; }
        ID GetLocalActorID() const noexcept { return m_LocalActorID; }

    protected:
        static ID currentID;

        ID m_ID;
        ID m_LocalActorID;
    };
}