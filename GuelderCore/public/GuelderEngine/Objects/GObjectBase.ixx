module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Core:GObjectBase;

import GuelderEngine.Core.Types;

export namespace GuelderEngine
{
    class GObjectBase// : INHERIT_GClass(GObjectBase)
    {
    public:
        using ID = size_t;
    public:
        DECLARE_DEFAULT(GObjectBase);

    protected:
        ID m_ID;
    private:
        using Type = GObjectBase;
    };
}