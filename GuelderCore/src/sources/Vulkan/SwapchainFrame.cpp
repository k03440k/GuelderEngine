module;
#include <vulkan/vulkan.hpp>

#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :SwapchainFrame;

import :SwapchainFrameSync;
import :CommandPool;
import GuelderEngine.Core.Types;

//Ctors
namespace GuelderEngine::Vulkan
{
    SwapchainFrame::SwapchainFrame(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo, const CommandPool& pool)
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
    void SwapchainFrame::Cleanup(const vk::Device& device, const CommandPool& pool) const noexcept
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
    void SwapchainFrame::FreeCommandBuffer(const CommandPool& pool, const vk::Device& device) const noexcept
    {
        pool.FreeCommandBuffer(device, commandBuffer);
    }
}
namespace GuelderEngine::Vulkan
{
    void SwapchainFrame::CreateFrameBuffer(const vk::Device& device, const vk::FramebufferCreateInfo& info)
    {
        framebuffer = device.createFramebuffer(info);
    }
    void SwapchainFrame::CreateFrameBuffer(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent)
    {
        const std::vector attachments{imageView};//idk

        const vk::FramebufferCreateInfo framebufferInfo(
            vk::FramebufferCreateFlags(),
            renderPass,
            attachments.size(),
            attachments.data(),
            swapchainExtent.width,
            swapchainExtent.height,
            1
        );
        
        CreateFrameBuffer(device, framebufferInfo);
    }
    void SwapchainFrame::CreateImage(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo)
    {
        this->image = viewInfo.image;
        imageView = device.createImageView(viewInfo);
    }
    void SwapchainFrame::CreateCommandBuffer(const vk::Device& device, const CommandPool& pool)
    {
        commandBuffer = pool.CreateCommandBuffer(device);
    }

    void SwapchainFrame::WaitForImage(const vk::Device& device, const uint64_t& delay) const
    {
        GE_CORE_CLASS_ASSERT(device.waitForFences(1, &sync.m_InFlightFence, VK_TRUE, delay) == vk::Result::eSuccess, "cannot wait for fence");
    }
    void SwapchainFrame::ResetFence(const vk::Device& device) const
    {
        GE_CORE_CLASS_ASSERT(device.resetFences(1, &sync.m_InFlightFence) == vk::Result::eSuccess, "cannot reset fence");
    }

    void SwapchainFrame::Recreate(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent,
        const vk::ImageViewCreateInfo& viewInfo, const CommandPool& pool)
    {
        CleanupImageView(device);
        CleanupFramebuffer(device);
        FreeCommandBuffer(pool, device);

        CreateImage(device, viewInfo);
        CreateFrameBuffer(device, renderPass, swapchainExtent);
        CreateCommandBuffer(device, pool);
    }
}