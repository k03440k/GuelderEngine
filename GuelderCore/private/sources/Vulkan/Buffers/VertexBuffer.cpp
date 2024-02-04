module;
#include <vulkan/vulkan.hpp>
#include "../../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :VertexBuffer;

import :IBuffer;
import :StagingBuffer;
import :QueueFamilyIndices;
import :Mesh;
import :DeviceManager;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan::Buffers
{
    VertexBuffer::VertexBuffer(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const QueueFamilyIndices& queueFamilyIndices,
        const vk::CommandPool& transferPool,
        const vk::Queue& transferQueue,
        const void* data,
        const uint& sizeOfAllData
    )
    {
        m_Size = sizeOfAllData;
        if(m_Size)
        {
            vk::BufferCreateInfo info{ vk::BufferCreateFlagBits(),
                sizeOfAllData,
                vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst
            };

            if(queueFamilyIndices.GetGraphicsFamily() != queueFamilyIndices.GetTransferFamily())
            {
                const uint uniqueIndices[] = { queueFamilyIndices.GetGraphicsFamily(), queueFamilyIndices.GetTransferFamily() };
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
                    IBuffer::FindMemoryType(
                        physicalDevice,
                        memRequirements.memoryTypeBits,
                        vk::MemoryPropertyFlagBits::eDeviceLocal
                    )
            };

            m_BufferMemory = device.allocateMemory(allocInfo);
            device.bindBufferMemory(m_Buffer, m_BufferMemory, 0);

            const auto stagingBuffer = StagingBuffer(device, physicalDevice, queueFamilyIndices, data, sizeOfAllData);
            CopyBuffer(stagingBuffer.GetBuffer(), m_Buffer, m_Size, device, transferPool, transferQueue);
            stagingBuffer.Cleanup(device);
        }
    }
    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_Size = other.m_Size;

        other.Reset();
    }
    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_Size = other.m_Size;

        other.Reset();

        return *this;
    }
    void VertexBuffer::Reset() noexcept
    {
        m_Buffer = nullptr;
        m_BufferMemory = nullptr;
        m_Size = 0;
    }
    void VertexBuffer::Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const
    {
        cmdBuffer.bindVertexBuffers(0, 1, &m_Buffer, &offset);
    }
}