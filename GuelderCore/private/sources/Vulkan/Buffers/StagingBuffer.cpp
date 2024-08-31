module;
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :StagingBuffer;

import :Buffer;
import :QueueFamilyIndices;
import :DeviceManager;

namespace GuelderEngine::Vulkan::Buffers
{
    StagingBuffer::StagingBuffer(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const QueueFamilyIndices& indices,
        const void* data,
        const uint& instanceSize,
        const uint& instanceCount,
        const uint& minOffsetAlignment
    )
        : Buffer(
                device,
                physicalDevice,
                indices,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                instanceSize,
                instanceCount,
                minOffsetAlignment
            )
        {
            if (m_Size)
            {
                MapMemory(device, m_Size);
                WriteToBuffer(data, m_Size);
                UnmapMemory(device);
            }
        }
}