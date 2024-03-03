module;
#include <glm/glm.hpp>
export module GuelderEngine.Actors:Actor2D;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;

export namespace GuelderEngine
{
    using Actor2DTransform = RenderActorTransform<2>;
    using Actor2DCreateInfo = RenderActorCreateInfo<2>;
    using Actor2D = RenderActor<2>;

    template<>
    Mat<2> MatFromRenderActorTransform(const Actor2DTransform& actorTransform)
    {
        const float _sin = glm::sin(actorTransform.rotation);
        const float _cos = glm::cos(actorTransform.rotation);
        const glm::mat2 rotationMatrix{ {_cos, _sin}, { -_sin, _cos } };

        const glm::mat2 scaleMatrix{ {actorTransform.scale.x, 0.0f}, { 0.0f, actorTransform.scale.y } };

        return scaleMatrix * rotationMatrix;
    }
}