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
        ubyte familyIndex{};

        switch(queueType)
        {
        case vk::QueueFlagBits::eGraphics:
            familyIndex = static_cast<ubyte>(queueFamilyIndices.GetGraphicsFamily());
            break;
        case vk::QueueFlagBits::eTransfer:
            familyIndex = static_cast<ubyte>(queueFamilyIndices.GetTransferFamily());
            break;
        default:
            GE_THROW("invalid queue type");
        }
        const vk::CommandPoolCreateInfo poolInfo(
            vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            static_cast<uint>(familyIndex)
        );

        m_CommandPool = device.createCommandPool(poolInfo);
    }

    CommandPool::CommandPool(CommandPool&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;

        other.Reset();
    }
    CommandPool& CommandPool::operator=(CommandPool&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void CommandPool::Reset() noexcept
    {
        m_CommandPool = nullptr;
    }
    void CommandPool::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyCommandPool(m_CommandPool);
    }

    vk::CommandBuffer CommandPool::CreateCommandBuffer(const vk::Device& device, const vk::CommandPool& commandPool)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            commandPool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        return device.allocateCommandBuffers(bufferInfo)[0];
    }
    vk::CommandBuffer CommandPool::BeginSingleTimeSubmitCommandBuffer(const vk::Device& device, const vk::CommandPool& commandPool)
    {
        const auto commandBuffer = CreateCommandBuffer(device, commandPool);

        const vk::CommandBufferBeginInfo beginInfo{
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit
        };

        commandBuffer.begin(beginInfo);

        return commandBuffer;
    }
    void CommandPool::FreeCommandBuffer(const vk::Device& device, const vk::CommandPool& commandPool, const vk::CommandBuffer& commandBuffer) noexcept
    {
        device.freeCommandBuffers(commandPool, 1, &commandBuffer);
    }

    vk::CommandBuffer CommandPool::CreateCommandBuffer(const vk::Device& device) const
    {
        return CreateCommandBuffer(device, m_CommandPool);
    }
    vk::CommandBuffer CommandPool::BeginSingleTimeSubmitCommandBuffer(const vk::Device& device) const
    {
        return BeginSingleTimeSubmitCommandBuffer(device, m_CommandPool);
    }
    void CommandPool::FreeCommandBuffer(const vk::Device& device, const vk::CommandBuffer& commandBuffer) const noexcept
    {
        FreeCommandBuffer(device, m_CommandPool, commandBuffer);
    }
}
namespace GuelderEngine::Vulkan
{
    const vk::CommandPool& CommandPool::GetCommandPool() const noexcept
    {
        return m_CommandPool;
    }
}