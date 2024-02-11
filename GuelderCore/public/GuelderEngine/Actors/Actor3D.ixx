module;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
export module GuelderEngine.Actors:Actor3D;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;

export namespace GuelderEngine
{
    using Actor3DTransform = RenderActorTransform<3, Vector3>;
    using Actor3DCreateInfo = RenderActorCreateInfo<3, Vector3>;
    using Actor3D = RenderActor<3, Vector3>;
    using CameraActor3D = CameraActor<3, Vector3>;

    template<>
    Mat4 MatFromRenderActorTransform(const Actor3DTransform& actorTransform)
    {
        /*auto transform = glm::translate(Mat4{ 1.f }, actorTransform.translation);

        transform = glm::rotate(transform, actorTransform.rotation.y, { 0.f, 1.f, 0.f });
        transform = glm::rotate(transform, actorTransform.rotation.x, { 1.f, 0.f, 0.f });
        transform = glm::rotate(transform, actorTransform.rotation.z, { 0.f, 0.f, 1.f });

        transform = glm::scale(transform, actorTransform.scale);

        return transform;*/
        const float c3 = glm::cos(actorTransform.rotation.z);
        const float s3 = glm::sin(actorTransform.rotation.z);
        const float c2 = glm::cos(actorTransform.rotation.x);
        const float s2 = glm::sin(actorTransform.rotation.x);
        const float c1 = glm::cos(actorTransform.rotation.y);
        const float s1 = glm::sin(actorTransform.rotation.y);
        return glm::mat4{
            {
                actorTransform.scale.x * (c1 * c3 + s1 * s2 * s3),
                actorTransform.scale.x * (c2 * s3),
                actorTransform.scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                actorTransform.scale.y * (c3 * s1 * s2 - c1 * s3),
                actorTransform.scale.y * (c2 * c3),
                actorTransform.scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                actorTransform.scale.z * (c2 * s1),
                actorTransform.scale.z * (-s2),
                actorTransform.scale.z * (c1 * c2),
                0.0f,
            },
            {actorTransform.translation.x, actorTransform.translation.y, actorTransform.translation.z, 1.0f} };
    }
}