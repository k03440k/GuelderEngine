module;
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :VertexBuffer;

import :DeviceManager;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    VertexBuffer::VertexBuffer(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const Mesh_t& mesh)
        : m_VerticesCount(mesh.size())
    {
        if(m_VerticesCount)
        {
            const vk::BufferCreateInfo info{vk::BufferCreateFlagBits(),
                sizeof(mesh[0])* mesh.size(),
                vk::BufferUsageFlagBits::eVertexBuffer,
                vk::SharingMode::eExclusive
            };

            m_Buffer = device.createBuffer(info);

            const auto memRequirements = device.getBufferMemoryRequirements(m_Buffer);

            vk::MemoryAllocateInfo allocInfo{
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
    VertexBuffer::VertexBuffer(const VertexBuffer& other)
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_VerticesCount = other.m_VerticesCount;
    }
    VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other)
    {
        if(this == &other)
            return *this;

        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_VerticesCount = other.m_VerticesCount;

        return *this;
    }
    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_VerticesCount = other.m_VerticesCount;

        other.Reset();
    }
    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_VerticesCount = other.m_VerticesCount;

        other.Reset();

        return *this;
    }
    void VertexBuffer::Reset() noexcept
    {
        m_Buffer = nullptr;
        m_BufferMemory = nullptr;
    }
    void VertexBuffer::Cleanup(const vk::Device& device) const noexcept
    {
        device.freeMemory(m_BufferMemory);
        device.destroyBuffer(m_Buffer);
    }
    void VertexBuffer::Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const
    {
        cmdBuffer.bindVertexBuffers(0, 1, &m_Buffer, &offset);
    }
    Types::uint VertexBuffer::GetVerticesCount() const noexcept { return m_VerticesCount; }
}