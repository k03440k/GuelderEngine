module;
#include <vulkan/vulkan.hpp>
#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :IBuffer;

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

        const vk::CommandBufferBeginInfo beginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
        GE_ASSERT_FN(cBuffer.begin(&beginInfo) == vk::Result::eSuccess, "cannot begin command buffer when copying buffer");

        const vk::BufferCopy copy{0, 0, size};
        cBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copy);
        cBuffer.end();

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cBuffer;

        GE_ASSERT_FN(transferQueue.submit(1, &submitInfo, nullptr) == vk::Result::eSuccess, "cannot submit on transfer queue");
        transferQueue.waitIdle();

        device.freeCommandBuffers(transferPool, 1, &cBuffer);
    }
    void IBuffer::Reset() noexcept
    {
        m_Buffer = nullptr;
        m_BufferMemory = nullptr;
        m_Size = 0;
    }
    void IBuffer::Cleanup(const vk::Device& device) const noexcept
    {
        device.freeMemory(m_BufferMemory);
        device.destroyBuffer(m_Buffer);
    }
    void IBuffer::Cleanup(const vk::Device& device, const std::vector<vk::Queue> queuesToWait) const noexcept
    {
        for(auto& queue : queuesToWait)
            queue.waitIdle();
        device.freeMemory(m_BufferMemory);
        device.destroyBuffer(m_Buffer);
    }
    const vk::Buffer& IBuffer::GetBuffer() const noexcept
    {
        return m_Buffer;
    }
    const vk::DeviceMemory& IBuffer::GetBufferMemory() const noexcept
    {
        return m_BufferMemory;
    }
    vk::DeviceSize IBuffer::GetSize() const noexcept
    {
        return m_Size;
    }
    bool IBuffer::Initialized() const noexcept
    {
        return m_Size;
    }
    uint IBuffer::FindMemoryType(const vk::PhysicalDevice& physicalDevice, const uint& typeFilter, const vk::MemoryPropertyFlags& properties)
    {
        const auto memProperties = physicalDevice.getMemoryProperties();

        for(uint i = 0; i < memProperties.memoryTypeCount; ++i)
        {
            if(typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        GE_CLASS_THROW("Failed to find suitable memory type");
    }
}