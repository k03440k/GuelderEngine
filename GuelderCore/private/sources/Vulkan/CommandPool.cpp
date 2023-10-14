module;
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan;
import :CommandPool;

import :SwapchainFrame;
import :QueueFamilyIndices;

import <vector>;

namespace GuelderEngine::Vulkan
{
    CommandPool::CommandPool(const vk::Device& device, const QueueFamilyIndices& queueFamilyIndices, const vk::QueueFlagBits& queueType)
    {
        uint familyIndex{};
        switch(queueType)
        {
        case vk::QueueFlagBits::eGraphics:
            familyIndex = queueFamilyIndices.GetGraphicsFamily();
            break;
        case vk::QueueFlagBits::eTransfer:
            familyIndex = queueFamilyIndices.GetTransferFamily();
            break;
            default:
                GE_CLASS_THROW("invalid queue type");
        }
        const vk::CommandPoolCreateInfo poolInfo(
            vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            familyIndex
        );

        m_CommandPool = device.createCommandPool(poolInfo);
    }
    CommandPool::CommandPool(const vk::Device& device, const QueueFamilyIndices& queueFamilyIndices, std::vector<SwapchainFrame>& frames)
        : CommandPool(device, queueFamilyIndices)
    {
        //m_CommandPool = MakePool(device, queueFamilyIndices);
        //m_CommandPool = MakeBuffer(device, m_CommandPool);
        MakeCommandBuffersForFrames(device, m_CommandPool, frames);
    }
    CommandPool::CommandPool(const CommandPool& other)
    {
        m_CommandPool = other.m_CommandPool;
        //m_CommandPool = other.m_CommandPool;
    }
    CommandPool::CommandPool(CommandPool&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;
        //m_CommandPool = other.m_CommandPool;

        other.Reset();
    }
    CommandPool& CommandPool::operator=(const CommandPool& other)
    {
        if(this == &other)
            return *this;

        m_CommandPool = other.m_CommandPool;
        //m_CommandPool = other.m_CommandPool;

        return *this;
    }
    CommandPool& CommandPool::operator=(CommandPool&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;
        //m_CommandPool = other.m_CommandPool;

        other.Reset();

        return *this;
    }
    void CommandPool::Reset() noexcept
    {
        m_CommandPool = nullptr;
        //m_CommandPool = nullptr;
    }
    void CommandPool::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyCommandPool(m_CommandPool);
    }
    vk::CommandBuffer CommandPool::MakeCommandBuffer(const vk::Device& device, const vk::CommandPool& pool)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        return device.allocateCommandBuffers(bufferInfo)[0];
    }
    vk::CommandBuffer CommandPool::CreateCommandBuffer(const vk::Device& device) const
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            m_CommandPool,
            vk::CommandBufferLevel::ePrimary,
            1
        );
        
        return device.allocateCommandBuffers(bufferInfo)[0];
    }
    void CommandPool::MakeCommandBuffersForFrames(const vk::Device& device, const vk::CommandPool& pool, std::vector<SwapchainFrame>& frames)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        for(auto&& frame : frames)
            frame.commandBuffer = device.allocateCommandBuffers(bufferInfo)[0];
    }
    void CommandPool::MakeCommandBuffersForFrames(const vk::Device& device, std::vector<SwapchainFrame>& frames) const
    {
        MakeCommandBuffersForFrames(device, m_CommandPool, frames);
    }
    void CommandPool::FreeCommandBuffer(const vk::Device& device, const vk::CommandBuffer& commandBuffer) const noexcept
    {
        device.freeCommandBuffers(m_CommandPool, 1, &commandBuffer);
    }
    const vk::CommandPool& CommandPool::GetCommandPool() const noexcept
    {
        return m_CommandPool;
    }
}