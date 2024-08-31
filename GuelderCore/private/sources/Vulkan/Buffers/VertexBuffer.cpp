module;
#include <vulkan/vulkan.hpp>
#include "../../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :VertexBuffer;

import GuelderEngine.Core.Types;
import :Buffer;
import :StagingBuffer;
import :QueueFamilyIndices;
import :Mesh;
import :DeviceManager;

namespace GuelderEngine::Vulkan::Buffers
{
    VertexBuffer::VertexBuffer(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const QueueFamilyIndices& queueFamilyIndices,
        const vk::CommandPool& transferPool,
        const vk::Queue& transferQueue,
        const void* data,
        const uint& sizeOfVertex,
        const uint& countOfVertices,
        const uint& minOffsetAlignment
    )
        : Buffer(
            device,
            physicalDevice,
            queueFamilyIndices,
            vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            sizeOfVertex,
            countOfVertices,
            minOffsetAlignment
        )
    {
        const auto stagingBuffer = StagingBuffer(device, physicalDevice, queueFamilyIndices, data, sizeOfVertex, countOfVertices, minOffsetAlignment);
        CopyBuffer(stagingBuffer.GetBuffer(), m_Buffer, m_Size, device, transferPool, transferQueue);
        stagingBuffer.Cleanup(device);
    }
    void VertexBuffer::Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const
    {
        cmdBuffer.bindVertexBuffers(0, 1, &m_Buffer, &offset);
    }
}