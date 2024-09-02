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

        RenderActorTransform(const Vec& translation = {}, const Vec& scale = {}, const Vec& position = {}, const RotationType& rotation = {}, const Vec& localPosition = {})
            : translation(translation), scale(scale), position(position), localPosition(localPosition), rotation(rotation) {}
        ~RenderActorTransform() = default;

        Vec translation;
        Vec scale;
        Vec position;
        Vec localPosition;
        RotationType rotation;
    };
}