module;
#include <glm/glm.hpp>
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
export module GuelderEngine.Actors:Actor2D;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;

export namespace GuelderEngine
{
    using Actor2DTransform = RenderActorTransform<2>;
    template<>
    Mat<2> MatFromRenderActorTransform(const Actor2DTransform& actorTransform)
    {
        const float _sin = glm::sin(actorTransform.rotation);
        const float _cos = glm::cos(actorTransform.rotation);
        const glm::mat2 rotationMatrix{ {_cos, _sin}, { -_sin, _cos } };

        const glm::mat2 scaleMatrix{ {actorTransform.scale.x, 0.0f}, { 0.0f, actorTransform.scale.y } };

        return scaleMatrix * rotationMatrix;
    }
    /*struct Object2DTransform : RenderActorTransform<2>
    {
        Object2DTransform()
        {
            scale = { 1.f, 1.f };
        }
        Object2DTransform(const Mat2& translation, const Vector2& scale, const Vector2& position, float rotation)
            : RenderActorTransform<2>(translation, scale, position, rotation) {}

        Mat2 Mat() const noexcept override;
    };*/
    struct Actor2DCreateInfo
    {
        const Actor2DTransform& transform;
        const Vulkan::Mesh2D& mesh;
    };
    using Actor2D = RenderActor<2>;
    //class Actor2D : public RenderActor2D
    //{
    //public:
    //    Actor2D() = default;
    //    virtual ~Actor2D() = default;

    //    Actor2D(const Vulkan::Buffers::VertexBuffer2D& vertexBuffer, const Vulkan::Buffers::IndexBuffer& indexBuffer);
    //    Actor2D(const Actor2DCreateInfo& info);

    //    void SetMesh(const Vulkan::Mesh2D& mesh);

    //    //Object2DTransform transform;

    //    const Vulkan::Mesh2D& GetMesh() const noexcept;
    //private:
    //    Vulkan::Mesh2D mesh;
    //};
}