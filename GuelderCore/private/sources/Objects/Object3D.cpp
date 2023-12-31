module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
//#include "../Utils/Debug.hpp"
#include <glm/glm.hpp>
module GuelderEngine.Game;
import :Object3D;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :Actor;

namespace GuelderEngine
{
    Object3D::Object3D(const Vulkan::Buffers::VertexBuffer3D& vertexBuffer, const Vulkan::Buffers::IndexBuffer& indexBuffer)
        : RenderActor(vertexBuffer, indexBuffer) {}
    Object3D::Object3D(const Object3DCreateInfo& info)
        : RenderActor(info.transform), mesh(info.mesh) {}

    void Object3D::SetMesh(const Vulkan::Mesh3D& mesh)
    {
        this->mesh = mesh;
    }
    const Vulkan::Mesh3D& Object3D::GetMesh() const noexcept
    {
        return mesh;
    }
    
    /*Mat3 Object3DTransform::Mat() const noexcept
    {
        return Mat3();
    }*/
}