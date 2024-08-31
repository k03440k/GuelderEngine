module;
#include <vulkan/vulkan.hpp>
#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :Buffer;

import :CommandPool;

namespace GuelderEngine::Vulkan::Buffers
{
    void CopyBuffer(
        const vk::Buffer& srcBuffer,
        const vk::Buffer& dstBuffer,
        const vk::DeviceSize& size,
        const vk::Device& device,
        const vk::CommandPool& transferPool,
        const vk::Queue& transferQueue
    )
    {
        const vk::CommandBufferAllocateInfo allocInfo(
            transferPool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        const auto cBuffer = device.allocateCommandBuffers(allocInfo)[0];

        const vk::CommandBufferBeginInfo beginInfo{ vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
        GE_ASSERT_FUNCTION(cBuffer.begin(&beginInfo) == vk::Result::eSuccess, "cannot begin command buffer when copying buffer");

        const vk::BufferCopy copy{ 0, 0, size };
        cBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copy);
        cBuffer.end();

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cBuffer;

        GE_ASSERT_FUNCTION(transferQueue.submit(1, &submitInfo, nullptr) == vk::Result::eSuccess, "cannot submit on transfer queue");
        transferQueue.waitIdle();

        device.freeCommandBuffers(transferPool, 1, &cBuffer);
    }
    Buffer::Buffer(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const QueueFamilyIndices& queueFamilyIndices,
        const vk::BufferUsageFlags& usageFlags,
        const vk::MemoryPropertyFlags& memoryProperty,
        const uint& instanceSize,
        const uint& instanceCount,
        const uint& minOffsetAlignment
    )
        : m_UsageFlags(usageFlags),
        m_MemoryPropertyFlags(memoryProperty),
        m_InstanceSize(instanceSize),
        m_InstanceCount(instanceCount),
        m_AlignmentSize(minOffsetAlignment),
        m_MappedMemory(nullptr)
    {
        m_AlignmentSize = GetAlignment(m_InstanceSize, minOffsetAlignment);
        m_Size = m_AlignmentSize * m_InstanceCount;
        if (m_Size)
        {
            vk::BufferCreateInfo info{ vk::BufferCreateFlagBits(),
                m_Size,
                usageFlags
            };

            if (queueFamilyIndices.GetGraphicsFamily() != queueFamilyIndices.GetTransferFamily())
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
                    FindMemoryType(
                        physicalDevice,
                        memRequirements.memoryTypeBits,
                        memoryProperty
                    )
            };

            m_BufferMemory = device.allocateMemory(allocInfo);
            device.bindBufferMemory(m_Buffer, m_BufferMemory, 0);
        }
    }
    Buffer::Buffer(Buffer&& other) noexcept
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_Size = other.m_Size;
        m_MappedMemory = other.m_MappedMemory;
        m_InstanceSize = other.m_InstanceSize;
        m_InstanceCount = other.m_InstanceCount;
        m_AlignmentSize = other.m_AlignmentSize;
        m_UsageFlags = other.m_UsageFlags;
        m_MemoryPropertyFlags = other.m_MemoryPropertyFlags;


        other.Reset();
    }
    Buffer& Buffer::operator=(Buffer&& other) noexcept
    {
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_Size = other.m_Size;
        m_MappedMemory = other.m_MappedMemory;
        m_InstanceSize = other.m_InstanceSize;
        m_InstanceCount = other.m_InstanceCount;
        m_AlignmentSize = other.m_AlignmentSize;
        m_UsageFlags = other.m_UsageFlags;
        m_MemoryPropertyFlags = other.m_MemoryPropertyFlags;

        other.Reset();

        return *this;
    }
    void Buffer::WriteToBuffer(const void* data, const vk::DeviceSize& size, const vk::DeviceSize& offset) const
    {
        if (size == VK_WHOLE_SIZE)
            memcpy(m_MappedMemory, data, m_Size);
        else
            memcpy(static_cast<char*>(m_MappedMemory) + offset, data, size);
    }
    void Buffer::WriteToIndex(const void* data, const uint& index) const
    {
        WriteToBuffer(data, m_InstanceSize, index * m_AlignmentSize);
    }
    void Buffer::Flush(const vk::Device& device, const vk::DeviceSize& size, const vk::DeviceSize& offset) const
    {
        const vk::MappedMemoryRange mappedRange{ m_BufferMemory, offset, size };

        GE_ASSERT(device.flushMappedMemoryRanges(1, &mappedRange) == vk::Result::eSuccess, "Failed to flush mapped mempry ranges.");
    }
    void Buffer::FlushIndex(const vk::Device& device, const uint& index) const
    {
        Flush(device, m_AlignmentSize, index * m_AlignmentSize);
    }
    void Buffer::Invalidate(const vk::Device& device, const vk::DeviceSize& size, const vk::DeviceSize& offset) const
    {
        const vk::MappedMemoryRange mappedRange{ m_BufferMemory, offset, size };

        GE_ASSERT(device.invalidateMappedMemoryRanges(1, &mappedRange) == vk::Result::eSuccess, "Failed to invalidate mapped mempry ranges.");
    }
    void Buffer::InvalidateIndex(const vk::Device& device, const uint& index) const
    {
        Invalidate(device, index * m_AlignmentSize);
    }
    void Buffer::MapMemory(const vk::Device& device, const vk::DeviceSize& size, const vk::DeviceSize& offset)
    {
        vk::Result result;
        if (size == VK_WHOLE_SIZE)
            result = device.mapMemory(m_BufferMemory, 0, m_Size, vk::MemoryMapFlagBits{}, &m_MappedMemory);
        else
            result = device.mapMemory(m_BufferMemory, offset, size, vk::MemoryMapFlagBits{}, &m_MappedMemory);

        GE_ASSERT(result == vk::Result::eSuccess, "Failed to map buffer memory.");
    }
    void Buffer::UnmapMemory(const vk::Device& device) const
    {
        device.unmapMemory(m_BufferMemory);
    }
    uint Buffer::FindMemoryType(const vk::PhysicalDevice& physicalDevice, const uint& typeFilter, const vk::MemoryPropertyFlags& properties)
    {
        const auto memProperties = physicalDevice.getMemoryProperties();

        for (uint i = 0; i < memProperties.memoryTypeCount; ++i)
        {
            if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        GE_CLASS_THROW("Failed to find suitable memory type");
    }
    uint Buffer::GetAlignment(const uint& instanceSize, const uint& minOffsetAlignment)
    {
        if (minOffsetAlignment > 0)
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        else
            return instanceSize;
    }
    void Buffer::Reset() noexcept
    {
        m_Buffer = nullptr;
        m_BufferMemory = nullptr;
        m_Size = 0;
        m_MappedMemory = nullptr;
        m_InstanceSize = 0;
        m_InstanceCount = 0;
        m_AlignmentSize = 0;
        m_UsageFlags = vk::BufferUsageFlags{};
        m_MemoryPropertyFlags = vk::MemoryPropertyFlags{};
    }
    void Buffer::Cleanup(const vk::Device& device) const noexcept
    {
        device.freeMemory(m_BufferMemory);
        device.destroyBuffer(m_Buffer);
    }
    void Buffer::Cleanup(const vk::Device& device, const std::vector<vk::Queue>& queuesToWait) const noexcept
    {
        for (auto& queue : queuesToWait)
            queue.waitIdle();

        device.freeMemory(m_BufferMemory);
        device.destroyBuffer(m_Buffer);
    }
    const vk::Buffer& Buffer::GetBuffer() const noexcept
    {
        return m_Buffer;
    }
    const vk::DeviceMemory& Buffer::GetBufferMemory() const noexcept
    {
        return m_BufferMemory;
    }
    vk::DeviceSize Buffer::GetSize() const noexcept
    {
        return m_Size;
    }
    vk::DescriptorBufferInfo Buffer::GetDescriptorBufferInfo(const vk::DeviceSize& size, const vk::DeviceSize& offset) const noexcept
    {
        return vk::DescriptorBufferInfo{ m_Buffer, offset, size };
    }
    vk::DescriptorBufferInfo Buffer::GetDescriptorBufferInfoForIndex(const uint& index) const noexcept
    {
        return GetDescriptorBufferInfo(m_AlignmentSize, index * m_AlignmentSize);
    }
    vk::BufferUsageFlags Buffer::GetBufferUsageFlags() const noexcept
    {
        return m_UsageFlags;
    }
    vk::MemoryPropertyFlags Buffer::GetMemoryPropertyFlags() const noexcept
    {
        return m_MemoryPropertyFlags;
    }
    uint Buffer::GetInstanceCount() const noexcept
    {
        return m_InstanceCount;
    }
    uint Buffer::GetInstanceSize() const noexcept
    {
        return m_InstanceSize;
    }
    uint Buffer::GetAlignmentSize() const noexcept
    {
        return m_AlignmentSize;
    }
    const void* Buffer::GetMappedMemory() const noexcept
    {
        return m_MappedMemory;
    }
    bool Buffer::IsInitialized() const noexcept
    {
        return m_Size;
    }
}