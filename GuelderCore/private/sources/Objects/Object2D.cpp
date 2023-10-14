module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
//#include "../Utils/Debug.hpp"
#include <glm/glm.hpp>
module GuelderEngine.Game;
import :Object2D;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;

namespace GuelderEngine
{
    Object2D::Object2D(const Vulkan::Buffers::VertexBuffer& vertexBuffer, const Vulkan::Buffers::IndexBuffer& indexBuffer)
        : RenderActor(vertexBuffer, indexBuffer)
    {
    }
    Object2D::Object2D(const Object2DCreateInfo& info)
        : transform(info.transform), mesh(info.mesh)
    {
    }
    void Object2D::SetMesh(const Vulkan::Mesh2D& mesh)
    {
        this->mesh = mesh;
    }
    const Vulkan::Mesh2D& Object2D::GetMesh() const noexcept
    {
        return mesh;
    }
    glm::mat2 Object2DTransform::Mat2() const noexcept
    {
        const float _sin = glm::sin(rotation);
        const float _cos = glm::cos(rotation);
        const glm::mat2 rotationMatrix{{_cos, _sin}, { -_sin, _cos }};

        const glm::mat2 scaleMatrix{{scale.x, 0.0f}, { 0.0f, scale.y }};

        return scaleMatrix * rotationMatrix;
    }
}