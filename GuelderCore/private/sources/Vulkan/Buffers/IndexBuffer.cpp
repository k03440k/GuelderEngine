module;
#include <vulkan/vulkan.hpp>
#include "../../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan;
import :IndexBuffer;

import :Buffer;
import :StagingBuffer;

namespace GuelderEngine::Vulkan::Buffers
{
    IndexBuffer::IndexBuffer(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const QueueFamilyIndices& queueFamilyIndices,
        const vk::CommandPool& transferPool,
        const vk::Queue& transferQueue,
        const Indices& indices
    )
    : Buffer(
            device,
            physicalDevice,
            queueFamilyIndices,
            vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            sizeof(indices[0]),
            indices.size()
        )
    {
        m_IndicesCount = indices.size();

        if(m_IndicesCount)
        {
            const auto stagingBuffer = Buffers::StagingBuffer(device, physicalDevice, queueFamilyIndices, indices.data(), sizeof(indices[0]), indices.size());
            CopyBuffer(stagingBuffer.GetBuffer(), m_Buffer, m_Size, device, transferPool, transferQueue);
            stagingBuffer.Cleanup(device);
        }
    }
    void IndexBuffer::Reset() noexcept
    {
        Buffer::Reset();
        m_IndicesCount = 0;
    }
    void IndexBuffer::Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const
    {
        cmdBuffer.bindIndexBuffer(m_Buffer, offset, vk::IndexType::eUint32);
    }
    uint IndexBuffer::GetIndicesCount() const noexcept
    {
        return m_IndicesCount;
    }
}