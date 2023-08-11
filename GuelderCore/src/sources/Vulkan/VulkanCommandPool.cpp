module;
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan;
import :VulkanCommandPool;

import :VulkanSwapchainFrame;
import :VulkanQueueFamilyIndices;

import <vector>;

namespace GuelderEngine::Vulkan
{
    VulkanCommandPool::VulkanCommandPool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices)
    {
        m_CommandPool = MakePool(device, queueFamilyIndices);
    }
    VulkanCommandPool::VulkanCommandPool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, std::vector<VulkanSwapchainFrame>& frames)
        : VulkanCommandPool(device, queueFamilyIndices)
    {
        //m_CommandPool = MakePool(device, queueFamilyIndices);
        //m_CommandPool = MakeBuffer(device, m_CommandPool);
        MakeCommandBuffersForFrames(device, m_CommandPool, frames);
    }
    VulkanCommandPool::VulkanCommandPool(const VulkanCommandPool& other)
    {
        m_CommandPool = other.m_CommandPool;
        //m_CommandPool = other.m_CommandPool;
    }
    VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;
        //m_CommandPool = other.m_CommandPool;

        other.Reset();
    }
    VulkanCommandPool& VulkanCommandPool::operator=(const VulkanCommandPool& other)
    {
        if(this == &other)
            return *this;

        m_CommandPool = other.m_CommandPool;
        //m_CommandPool = other.m_CommandPool;

        return *this;
    }
    VulkanCommandPool& VulkanCommandPool::operator=(VulkanCommandPool&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;
        //m_CommandPool = other.m_CommandPool;

        other.Reset();

        return *this;
    }
    void VulkanCommandPool::Reset() noexcept
    {
        m_CommandPool = nullptr;
        //m_CommandPool = nullptr;
    }
    void VulkanCommandPool::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyCommandPool(m_CommandPool);
    }
    vk::CommandPool VulkanCommandPool::MakePool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices)
    {
        const vk::CommandPoolCreateInfo poolInfo(
            vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queueFamilyIndices.graphicsFamily.value()
        );

        return device.createCommandPool(poolInfo);
    }
    vk::CommandBuffer VulkanCommandPool::MakeBuffer(const vk::Device& device, const vk::CommandPool& pool)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        return device.allocateCommandBuffers(bufferInfo)[0];
    }
    vk::CommandBuffer VulkanCommandPool::CreateCommandBuffer(const vk::Device& device) const
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            m_CommandPool,
            vk::CommandBufferLevel::ePrimary,
            1
        );
        
            return device.allocateCommandBuffers(bufferInfo)[0];
    }
    void VulkanCommandPool::MakeCommandBuffersForFrames(const vk::Device& device, const vk::CommandPool& pool, std::vector<VulkanSwapchainFrame>& frames)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        for(auto&& frame : frames)
            frame.commandBuffer = device.allocateCommandBuffers(bufferInfo)[0];
    }
    void VulkanCommandPool::MakeCommandBuffersForFrames(const vk::Device& device, std::vector<VulkanSwapchainFrame>& frames) const
    {
        MakeCommandBuffersForFrames(device, m_CommandPool, frames);
    }
    void VulkanCommandPool::FreeCommandBuffer(const vk::Device& device, const vk::CommandBuffer& commandBuffer) const noexcept
    {
        device.freeCommandBuffers(m_CommandPool, 1, &commandBuffer);
    }
}