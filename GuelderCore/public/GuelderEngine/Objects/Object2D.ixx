module;
#include <glm/glm.hpp>
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
export module GuelderEngine.Game:Object2D;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;

export namespace GuelderEngine
{
    using Object2DTransform = RenderActorTransform<2>;
    template<>
    Mat<2> MatFromRenderActorTransform(const Actor2D& actor) noexcept
    {
        const float _sin = glm::sin(actor.transform.rotation);
        const float _cos = glm::cos(actor.transform.rotation);
        const glm::mat2 rotationMatrix{ {_cos, _sin}, { -_sin, _cos } };

        const glm::mat2 scaleMatrix{ {actor.transform.scale.x, 0.0f}, { 0.0f, actor.transform.scale.y } };

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
    struct Object2DCreateInfo
    {
        const Object2DTransform& transform;
        const Vulkan::Mesh2D& mesh;
    };
    class Object2D : public Actor2D
    {
    public:
        Object2D() = default;
        virtual ~Object2D() = default;

        Object2D(const Vulkan::Buffers::VertexBuffer2D& vertexBuffer, const Vulkan::Buffers::IndexBuffer& indexBuffer);
        Object2D(const Object2DCreateInfo& info);

        void SetMesh(const Vulkan::Mesh2D& mesh);

        //Object2DTransform transform;

        const Vulkan::Mesh2D& GetMesh() const noexcept;
    private:
        Vulkan::Mesh2D mesh;
    };
}