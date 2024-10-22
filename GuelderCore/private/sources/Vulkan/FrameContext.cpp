module;
#include <vulkan/vulkan.hpp>
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :FrameContext;

import :FrameSync;
import :CommandPool;
import GuelderEngine.Core.Types;

import <array>;

namespace GuelderEngine::Vulkan
{
    FrameContext::FrameContext(
        const vk::Device& device,
        const vk::CommandPool& commandPool,
        const vk::RenderPass& renderPass,
        const std::vector<vk::ImageView>& attachments,
        const vk::Extent2D& extent
    )
    {
        commandBuffer = CreateCommandBuffer(device, commandPool);
        frameBuffer = CreateFrameBuffer(device, attachments, renderPass, extent);
    }
    FrameContext::FrameContext(FrameContext&& other) noexcept
    {
        frameBuffer = other.frameBuffer;
        commandBuffer = other.commandBuffer;

        other.Reset();
    }
    FrameContext& FrameContext::operator=(FrameContext&& other) noexcept
    {
        frameBuffer = other.frameBuffer;
        commandBuffer = other.commandBuffer;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void FrameContext::Reset() noexcept
    {
        frameBuffer = nullptr;
        commandBuffer = nullptr;
    }
    void FrameContext::Cleanup(const vk::Device& device, const vk::CommandPool& pool) const noexcept
    {
        device.freeCommandBuffers(pool, 1, &commandBuffer);
        device.destroyFramebuffer(frameBuffer);
    }

    vk::Framebuffer FrameContext::CreateFrameBuffer(const vk::Device& device, const std::vector<vk::ImageView>& attachments, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent)
    {
        const vk::FramebufferCreateInfo framebufferInfo(
            vk::FramebufferCreateFlags(),
            renderPass,
            attachments.size(),
            attachments.data(),
            swapchainExtent.width,
            swapchainExtent.height,
            1
        );

        return device.createFramebuffer(framebufferInfo);
    }
    vk::CommandBuffer FrameContext::CreateCommandBuffer(const vk::Device& device, const vk::CommandPool& pool)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        return device.allocateCommandBuffers(bufferInfo)[0];
    }

    void FrameContext::FreeCommandBuffer(const vk::CommandPool& commandPool, const vk::Device& device) const noexcept
    {
        device.freeCommandBuffers(commandPool, 1, &commandBuffer);
    }

    void FrameContext::Recreate(
        const vk::Device& device,
        const vk::RenderPass& renderPass,
        const std::vector<vk::ImageView>& attachments,
        const vk::CommandPool& commandPool,
        const vk::Extent2D& swapchainExtent
    )
    {
        device.freeCommandBuffers(commandPool, 1, &commandBuffer);
        device.destroyFramebuffer(frameBuffer);

        commandBuffer = CreateCommandBuffer(device, commandPool);
        frameBuffer = CreateFrameBuffer(device, attachments, renderPass, swapchainExtent);
    }
}