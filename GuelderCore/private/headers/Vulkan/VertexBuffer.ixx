module;
#include <vulkan/vulkan.hpp>
#include "../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VertexBuffer;

import :IVulkanObject;
import :Mesh;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    class VertexBuffer : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VertexBuffer);

        VertexBuffer(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const Mesh_t& mesh);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const;

        Types::uint GetVerticesCount() const noexcept;
    private:
        vk::Buffer m_Buffer;
        vk::DeviceMemory m_BufferMemory;

        Types::uint m_VerticesCount;
    };
}