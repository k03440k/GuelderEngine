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
    struct Object2DTransform
    {
        glm::mat2 translation;
        Vector2 scale{1.f, 1.f};
        Vector2 position;
        float rotation;

        glm::mat2 Mat2() const noexcept;
    };
    struct Object2DCreateInfo
    {
        const Object2DTransform& transform;
        const Vulkan::Mesh2D& mesh;
    };
    class Object2D : public RenderActor
    {
    public:
        Object2D() = default;
        virtual ~Object2D() = default;

        Object2D(const Vulkan::Buffers::VertexBuffer& vertexBuffer, const Vulkan::Buffers::IndexBuffer& indexBuffer);
        Object2D(const Object2DCreateInfo& info);

        void SetMesh(const Vulkan::Mesh2D& mesh);

        Object2DTransform transform;

        const Vulkan::Mesh2D& GetMesh() const noexcept;
    private:
        Vulkan::Mesh2D mesh;
    };
}