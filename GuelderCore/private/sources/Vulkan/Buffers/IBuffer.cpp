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
        //TODO: maybe make class CommandBuffer
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

    //void CopyBuffer(const Buffer& srcBuffer, Buffer& dstBuffer, const vk::Device& device, const vk::CommandPool& transferPool, const vk::Queue& transferQueue)
    //{
    //    GE_ASSERT_FN(srcBuffer.GetSize() == dstBuffer.GetSize(), "different buffers sizes");
    //    //TODO: maybe make class CommandBuffer
    //    vk::CommandBufferAllocateInfo allocInfo{
    //        transferPool,
    //        vk::CommandBufferLevel::ePrimary,
    //        1
    //    };

    //    vk::CommandBuffer cBuffer = device.allocateCommandBuffers(allocInfo, &cBuffer)[0];

    //    vk::CommandBufferBeginInfo beginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
    //    GE_ASSERT_FN(cBuffer.begin(&beginInfo) == vk::Result::eSuccess, "cannot begin command buffer when copying buffer");

    //    vk::BufferCopy copy{0, 0, dstBuffer.GetSize()};
    //    cBuffer.copyBuffer(srcBuffer.GetBuffer(), dstBuffer.GetBuffer(), 1, &copy);
    //    cBuffer.end();

    //    vk::SubmitInfo submitInfo{};
    //    submitInfo.commandBufferCount = 1;
    //    submitInfo.pCommandBuffers = &cBuffer;

    //    GE_ASSERT_FN(transferQueue.submit(1, &submitInfo, nullptr) == vk::Result::eSuccess, "cannot submit on transfer queue");
    //    transferQueue.waitIdle();

    //    device.freeCommandBuffers(transferPool, 1, &cBuffer);
    //}
}