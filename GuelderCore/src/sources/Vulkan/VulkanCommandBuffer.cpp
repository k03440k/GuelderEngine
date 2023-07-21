module;
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan;
import :VulkanCommandBuffer;

import :VulkanSwapchainFrame;

import <vector>;

namespace GuelderEngine::Vulkan
{
    VulkanCommandBuffer::VulkanCommandBuffer(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, const vk::SurfaceKHR& surface,
        std::vector<VulkanSwapchainFrame>& frames)
    {
        m_CommandPool = MakePool(device, queueFamilyIndices, surface);
        m_CommandBuffer = MakeBuffer(device, m_CommandPool, frames);
    }
    VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandBuffer& other)
    {
        m_CommandPool = other.m_CommandPool;
        m_CommandBuffer = other.m_CommandBuffer;
    }
    VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;
        m_CommandBuffer = other.m_CommandBuffer;

        other.Reset();
    }
    VulkanCommandBuffer& VulkanCommandBuffer::operator=(const VulkanCommandBuffer& other)
    {
        if(this == &other)
            return *this;

        m_CommandPool = other.m_CommandPool;
        m_CommandBuffer = other.m_CommandBuffer;

        return *this;
    }
    VulkanCommandBuffer& VulkanCommandBuffer::operator=(VulkanCommandBuffer&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;
        m_CommandBuffer = other.m_CommandBuffer;

        other.Reset();

        return *this;
    }
    void VulkanCommandBuffer::Reset() noexcept
    {
        m_CommandPool = nullptr;
        m_CommandBuffer = nullptr;
    }
    void VulkanCommandBuffer::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyCommandPool(m_CommandPool);
    }
    vk::CommandPool VulkanCommandBuffer::MakePool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, const vk::SurfaceKHR& surface)
    {
        const vk::CommandPoolCreateInfo poolInfo(
            vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queueFamilyIndices.graphicsFamily.value()
        );

        return device.createCommandPool(poolInfo);
    }
    vk::CommandBuffer VulkanCommandBuffer::MakeBuffer(const vk::Device& device, const vk::CommandPool& pool, std::vector<VulkanSwapchainFrame>& frames)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        for(auto&& frame : frames)
            frame.commandBuffer = device.allocateCommandBuffers(bufferInfo)[0];

        const vk::CommandBuffer buffer = device.allocateCommandBuffers(bufferInfo)[0];

        return buffer;
    }
}