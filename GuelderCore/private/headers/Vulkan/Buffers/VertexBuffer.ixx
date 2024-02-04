module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VertexBuffer;

import :IBuffer;
import :StagingBuffer;
import :QueueFamilyIndices;
import :Mesh;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan::Buffers
{
    class VertexBuffer final : public IBuffer
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(VertexBuffer);
        DECLARE_DEFAULT_COPYING(VertexBuffer);
        DECLARE_MOVING(VertexBuffer);

        VertexBuffer(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue,
            const void* data,
            const uint& sizeOfAllData
        );

        void Reset() noexcept override;

        void Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const;
    };
}