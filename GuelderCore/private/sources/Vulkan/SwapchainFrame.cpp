module;
#include <vulkan/vulkan.hpp>

#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :SwapchainFrame;

import :SwapchainFrameSync;
import :CommandPool;
import GuelderEngine.Core.Types;

import <array>;

//Ctors
namespace GuelderEngine::Vulkan
{
    SwapchainFrame::SwapchainFrame(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo, const vk::CommandPool& pool)
    {
        CreateImage(device, viewInfo);
        CreateCommandBuffer(device, pool);
        sync = SwapchainFrameSync(device);
    }
    SwapchainFrame::SwapchainFrame(const SwapchainFrame& other)
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;
        sync = other.sync;
    }
    SwapchainFrame::SwapchainFrame(SwapchainFrame&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;
        sync = std::forward<SwapchainFrameSync>(other.sync);

        other.Reset();
    }
    SwapchainFrame& SwapchainFrame::operator=(const SwapchainFrame& other)
    {
        if(this == &other)
            return *this;

        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;
        sync = other.sync;

        return *this;
    }
    SwapchainFrame& SwapchainFrame::operator=(SwapchainFrame&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;
        sync = std::forward<SwapchainFrameSync>(other.sync);

        other.Reset();

        return *this;
    }
}
//Cleanups
namespace GuelderEngine::Vulkan
{
    void SwapchainFrame::Reset() noexcept
    {
        image = nullptr;
        imageView = nullptr;
        framebuffer = nullptr;
        commandBuffer = nullptr;
        sync.Reset();
    }
    void SwapchainFrame::Cleanup(const vk::Device& device, const vk::CommandPool& pool) const noexcept
    {
        FreeCommandBuffer(pool, device);
        device.destroyImageView(imageView);
        CleanupFramebuffer(device);
        sync.Cleanup(device);
    }
    void SwapchainFrame::CleanupImageView(const vk::Device& device) const noexcept
    {
        device.destroyImageView(imageView);
    }
    void SwapchainFrame::CleanupFramebuffer(const vk::Device& device) const noexcept
    {
        device.destroyFramebuffer(framebuffer);
    }
    void SwapchainFrame::FreeCommandBuffer(const vk::CommandPool& pool, const vk::Device& device) const noexcept
    {
        device.freeCommandBuffers(pool, 1, &commandBuffer);
    }
}
namespace GuelderEngine::Vulkan
{
    void SwapchainFrame::CreateFrameBuffer(const vk::Device& device, const vk::ImageView& depthImageView, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent)
    {
        std::array attachments{imageView, depthImageView};

        const vk::FramebufferCreateInfo framebufferInfo(
            vk::FramebufferCreateFlags(),
            renderPass,
            attachments.size(),
            attachments.data(),
            swapchainExtent.width,
            swapchainExtent.height,
            1
        );
        
        framebuffer = device.createFramebuffer(framebufferInfo);
    }
    void SwapchainFrame::CreateImage(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo)
    {
        image = viewInfo.image;
        imageView = device.createImageView(viewInfo);
    }
    void SwapchainFrame::CreateCommandBuffer(const vk::Device& device, const vk::CommandPool& pool)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        commandBuffer = device.allocateCommandBuffers(bufferInfo)[0];
    }

    void SwapchainFrame::WaitForImage(const vk::Device& device, const uint64_t& delay) const
    {
        GE_CLASS_ASSERT(device.waitForFences(1, &sync.GetFlightFence(), VK_TRUE, delay) == vk::Result::eSuccess, "cannot wait for fence");
    }
    void SwapchainFrame::ResetFence(const vk::Device& device) const
    {
        GE_CLASS_ASSERT(device.resetFences(1, &sync.GetFlightFence()) == vk::Result::eSuccess, "cannot reset fence");
    }

    void SwapchainFrame::Recreate(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent,
        const vk::ImageViewCreateInfo& viewInfo, const vk::ImageView& depthImageView, const vk::CommandPool& pool)
    {
        CleanupImageView(device);
        CleanupFramebuffer(device);
        FreeCommandBuffer(pool, device);

        CreateImage(device, viewInfo);
        CreateFrameBuffer(device, depthImageView, renderPass, swapchainExtent);
        CreateCommandBuffer(device, pool);
    }
}