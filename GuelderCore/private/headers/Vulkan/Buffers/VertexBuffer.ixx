module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VertexBuffer;

import :IBuffer;
import :QueueFamilyIndices;
import :Mesh;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan::Buffers
{
    class VertexBuffer : public IBuffer
    {
    public:
        DECLARE_DCAD_AND_CAM(VertexBuffer);

        VertexBuffer(
            const vk::Device& device, 
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& indices,
            const Mesh_t& mesh
        );

        void Reset() noexcept override;

        void Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const;

        Types::uint GetVerticesCount() const noexcept;

    private:
        Types::uint m_VerticesCount;
    };
}