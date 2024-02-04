module;
#include <glm/glm.hpp>

#include "headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Actors:RenderActorTransform;

import GuelderEngine.Core.Types;

export namespace GuelderEngine
{
    template<uint dimension, typename RotationType = float>
    struct RenderActorTransform
    {
    public:
        using Vec = Vector<dimension>;
    public:
        DECLARE_DEFAULT_COPYING_AND_MOVING(RenderActorTransform);

        RenderActorTransform() = default;
        RenderActorTransform(const Vec& translation, const Vec& scale, const Vec& position, const RotationType& rotation)
            : translation(translation), scale(scale), position(position), rotation(rotation) {}
        ~RenderActorTransform() = default;

        Vec translation;
        Vec scale;
        Vec position;
        Vec localPosition;
        RotationType rotation;
    };
}