module;
#include <glm/glm.hpp>
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
export module GuelderEngine.Game:Object3D;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;
//TODO: start a video tutorial
export namespace GuelderEngine
{
    using Object3DTransform = RenderActorTransform<3>;
    template<>
    Mat<3> MatFromRenderActorTransform(const Actor3D& actor) noexcept
    {
        return glm::mat<3, 3, glm::f32>{};
    }
    /*struct Object3DTransform : RenderActorTransform<3>
    {
        Object3DTransform()
        {
            scale = { 1.f, 1.f, 1.f };
        }
        Object3DTransform(const Mat3& translation, const Vector3& scale, const Vector3& position, float rotation)
            : RenderActorTransform<3>(translation, scale, position, rotation) {}

        Mat3 Mat() const noexcept override;
    };*/
    struct Object3DCreateInfo
    {
        const Object3DTransform& transform;
        const Vulkan::Mesh3D& mesh;
    };
    class Object3D : public RenderActor<3>
    {
    public:
        Object3D() = default;
        virtual ~Object3D() = default;

        Object3D(const Vulkan::Buffers::VertexBuffer3D& vertexBuffer, const Vulkan::Buffers::IndexBuffer& indexBuffer);
        Object3D(const Object3DCreateInfo& info);

        void SetMesh(const Vulkan::Mesh3D& mesh);

        //Object3DTransform transform;

        const Vulkan::Mesh3D& GetMesh() const noexcept;
    private:
        Vulkan::Mesh3D mesh;
    };
}