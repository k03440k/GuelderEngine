//module;
//#include <vulkan/vulkan.hpp>
//#include "../../../headers/Core/GObject/GClass.hpp"
//module GuelderEngine.Vulkan;
//import :VertexBuffer;
//
//import :IBuffer;
//import :StagingBuffer;
//import :QueueFamilyIndices;
//import :Mesh;
//import :DeviceManager;
//import GuelderEngine.Core.Types;
//
//namespace GuelderEngine::Vulkan::Buffers
//{
//    VertexBuffer::VertexBuffer(
//        const vk::Device& device,
//        const vk::PhysicalDevice& physicalDevice,
//        const QueueFamilyIndices& queueFamilyIndices,
//        const vk::CommandPool& transferPool,
//        const vk::Queue& transferQueue,
//        const Vertices2D& mesh
//    )
//    VertexBuffer::VertexBuffer(const VertexBuffer& other)
//    {
//        m_Buffer = other.m_Buffer;
//        m_BufferMemory = other.m_BufferMemory;
//        m_VerticesCount = other.m_VerticesCount;
//        m_Size = other.m_Size;
//    }
//    VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other)
//    {
//        if(this == &other)
//            return *this;
//
//        m_Buffer = other.m_Buffer;
//        m_BufferMemory = other.m_BufferMemory;
//        m_VerticesCount = other.m_VerticesCount;
//        m_Size = other.m_Size;
//
//        return *this;
//    }
//    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
//    {
//        m_Buffer = other.m_Buffer;
//        m_BufferMemory = other.m_BufferMemory;
//        m_VerticesCount = other.m_VerticesCount;
//        m_Size = other.m_Size;
//
//        other.Reset();
//    }
//    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
//    {
//        m_Buffer = other.m_Buffer;
//        m_BufferMemory = other.m_BufferMemory;
//        m_VerticesCount = other.m_VerticesCount;
//        m_Size = other.m_Size;
//
//        other.Reset();
//
//        return *this;
//    }
//    void VertexBuffer::Reset() noexcept
//    {
//        m_Buffer = nullptr;
//        m_BufferMemory = nullptr;
//        m_VerticesCount = 0;
//        m_Size = 0;
//    }
//    void VertexBuffer::Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const
//    {
//        cmdBuffer.bindVertexBuffers(0, 1, &m_Buffer, &offset);
//    }
//    uint VertexBuffer::GetVerticesCount() const noexcept { return m_VerticesCount; }
//}