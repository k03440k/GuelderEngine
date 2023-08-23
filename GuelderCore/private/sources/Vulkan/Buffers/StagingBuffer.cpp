module;
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :StagingBuffer;

import :IBuffer;
import :QueueFamilyIndices;
import :DeviceManager;

namespace GuelderEngine::Vulkan::Buffers
{
    StagingBuffer::StagingBuffer(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const Mesh_t& mesh)
    {
        m_Size = sizeof(mesh[0]) * mesh.size();
        if(m_Size)
        {
            vk::BufferCreateInfo info{vk::BufferCreateFlagBits(),
                m_Size,
                vk::BufferUsageFlagBits::eTransferSrc
            };

            if(indices.GetGraphicsFamily() != indices.GetTransferFamily())
            {
                const Types::uint uniqueIndices[] = { indices.GetGraphicsFamily(), indices.GetTransferFamily() };
                info.queueFamilyIndexCount = 2;
                info.pQueueFamilyIndices = uniqueIndices;
                info.sharingMode = vk::SharingMode::eConcurrent;
            }
            else
                info.sharingMode = vk::SharingMode::eExclusive;

            m_Buffer = device.createBuffer(info);

            const auto memRequirements = device.getBufferMemoryRequirements(m_Buffer);

            const vk::MemoryAllocateInfo allocInfo{
                memRequirements.size,
                    DeviceManager::FindMemType(
                        physicalDevice,
                        memRequirements.memoryTypeBits,
                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
                    )
            };

            m_BufferMemory = device.allocateMemory(allocInfo);
            device.bindBufferMemory(m_Buffer, m_BufferMemory, 0);

            void* data = device.mapMemory(m_BufferMemory, 0, info.size);
            memcpy(data, mesh.data(), info.size);
            device.unmapMemory(m_BufferMemory);
        }
    }
    StagingBuffer::StagingBuffer(const StagingBuffer& other)
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_Size = other.m_Size;
    }
    StagingBuffer& StagingBuffer::operator=(const StagingBuffer& other)
    {
        if(this == &other)
            return *this;

        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_Size = other.m_Size;

        return *this;
    }
    StagingBuffer::StagingBuffer(StagingBuffer&& other) noexcept
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_Size = other.m_Size;

        other.Reset();
    }
    StagingBuffer& StagingBuffer::operator=(StagingBuffer&& other) noexcept
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_Size = other.m_Size;

        other.Reset();

        return *this;
    }
}