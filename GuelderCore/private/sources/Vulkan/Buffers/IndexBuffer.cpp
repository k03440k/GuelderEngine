module;
#include <vulkan/vulkan.hpp>
#include "../../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan;
import :IndexBuffer;

import :IBuffer;
import :StagingBuffer;
import :DeviceManager;

export namespace GuelderEngine::Vulkan::Buffers
{
    IndexBuffer::IndexBuffer(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const QueueFamilyIndices& queueFamilyIndices,
        const vk::CommandPool& transferPool,
        const vk::Queue& transferQueue,
        const Indices& indices
    )
    {
        m_IndicesCount = indices.size();
        m_Size = sizeof(indices[0]) * indices.size();

        if(m_IndicesCount)
        {
            vk::BufferCreateInfo info{vk::BufferCreateFlagBits(),
               m_Size,
               vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst
           };

            if(queueFamilyIndices.GetGraphicsFamily() != queueFamilyIndices.GetTransferFamily())
            {
                const Types::uint uniqueIndices[] = { queueFamilyIndices.GetGraphicsFamily(), queueFamilyIndices.GetTransferFamily() };
                info.queueFamilyIndexCount = 2;
                info.pQueueFamilyIndices = uniqueIndices;
                info.sharingMode = vk::SharingMode::eConcurrent;
            }
            else
                info.sharingMode = vk::SharingMode::eExclusive;

            m_Buffer = device.createBuffer(info);

            const auto memRequirements = device.getBufferMemoryRequirements(m_Buffer);

            vk::MemoryAllocateInfo allocInfo{
                memRequirements.size,
                    DeviceManager::FindMemType(
                        physicalDevice,
                        memRequirements.memoryTypeBits,
                        vk::MemoryPropertyFlagBits::eDeviceLocal
                    )
            };

            m_BufferMemory = device.allocateMemory(allocInfo);
            device.bindBufferMemory(m_Buffer, m_BufferMemory, 0);

            const auto stagingBuffer = Buffers::StagingBuffer<Types::uint>(device, physicalDevice, queueFamilyIndices, indices);
            CopyBuffer(stagingBuffer.GetBuffer(), m_Buffer, m_Size, device, transferPool, transferQueue);
            stagingBuffer.Cleanup(device);
        }
    }
    IndexBuffer::IndexBuffer(const IndexBuffer& other) noexcept
    {
        m_IndicesCount = other.m_IndicesCount;
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
    }
    IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other) noexcept
    {
        if(this == &other)
            return *this;

        m_IndicesCount = other.m_IndicesCount;
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;

        return *this;
    }
    void IndexBuffer::Reset() noexcept
    {
        m_IndicesCount = 0;
    }
    void IndexBuffer::Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const
    {
        cmdBuffer.bindIndexBuffer(m_Buffer, offset, vk::IndexType::eUint32);
    }
    Types::uint IndexBuffer::GetIndicesCount() const noexcept
    {
        return m_IndicesCount;
    }
}