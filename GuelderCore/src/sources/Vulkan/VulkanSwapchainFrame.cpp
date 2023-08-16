module;
#include <vulkan/vulkan.hpp>

#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :VulkanSwapchainFrame;

import :VulkanSync;
import :VulkanCommandPool;
import GuelderEngine.Core.Types;

//Ctors
namespace GuelderEngine::Vulkan
{
    VulkanSwapchainFrame::VulkanSwapchainFrame(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo, const VulkanCommandPool& pool)
    {
        CreateImage(device, viewInfo);
        CreateCommandBuffer(device, pool);
        sync = VulkanSwapchainFrameSync(device);
    }
    VulkanSwapchainFrame::VulkanSwapchainFrame(const VulkanSwapchainFrame& other)
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;
        sync = other.sync;
    }
    VulkanSwapchainFrame::VulkanSwapchainFrame(VulkanSwapchainFrame&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;
        sync = std::forward<VulkanSwapchainFrameSync>(other.sync);

        other.Reset();
    }
    VulkanSwapchainFrame& VulkanSwapchainFrame::operator=(const VulkanSwapchainFrame& other)
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
    VulkanSwapchainFrame& VulkanSwapchainFrame::operator=(VulkanSwapchainFrame&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;
        sync = std::forward<VulkanSwapchainFrameSync>(other.sync);

        other.Reset();

        return *this;
    }
}
//Cleanups
namespace GuelderEngine::Vulkan
{
    void VulkanSwapchainFrame::Reset() noexcept
    {
        image = nullptr;
        imageView = nullptr;
        framebuffer = nullptr;
        commandBuffer = nullptr;
        sync.Reset();
    }
    void VulkanSwapchainFrame::Cleanup(const vk::Device& device, const VulkanCommandPool& pool) const noexcept
    {
        FreeCommandBuffer(pool, device);
        device.destroyImageView(imageView);
        CleanupFramebuffer(device);
        sync.Cleanup(device);
    }
    void VulkanSwapchainFrame::CleanupImageView(const vk::Device& device) const noexcept
    {
        device.destroyImageView(imageView);
    }
    void VulkanSwapchainFrame::CleanupFramebuffer(const vk::Device& device) const noexcept
    {
        device.destroyFramebuffer(framebuffer);
    }
    void VulkanSwapchainFrame::FreeCommandBuffer(const VulkanCommandPool& pool, const vk::Device& device) const noexcept
    {
        pool.FreeCommandBuffer(device, commandBuffer);
    }
}
namespace GuelderEngine::Vulkan
{
    void VulkanSwapchainFrame::CreateFrameBuffer(const vk::Device& device, const vk::FramebufferCreateInfo& info)
    {
        framebuffer = device.createFramebuffer(info);
    }
    void VulkanSwapchainFrame::CreateFrameBuffer(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent)
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
    void VulkanSwapchainFrame::CreateImage(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo)
    {
        this->image = viewInfo.image;
        imageView = device.createImageView(viewInfo);
    }
    void VulkanSwapchainFrame::CreateCommandBuffer(const vk::Device& device, const VulkanCommandPool& pool)
    {
        commandBuffer = pool.CreateCommandBuffer(device);
    }

    void VulkanSwapchainFrame::WaitForImage(const vk::Device& device, const uint64_t& delay) const
    {
        GE_CORE_CLASS_ASSERT(device.waitForFences(1, &sync.m_InFlightFence, VK_TRUE, delay) == vk::Result::eSuccess, "cannot wait for fence");
    }
    void VulkanSwapchainFrame::ResetFence(const vk::Device& device) const
    {
        GE_CORE_CLASS_ASSERT(device.resetFences(1, &sync.m_InFlightFence) == vk::Result::eSuccess, "cannot reset fence");
    }

    void VulkanSwapchainFrame::Recreate(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent,
        const vk::ImageViewCreateInfo& viewInfo, const VulkanCommandPool& pool)
    {
        CleanupImageView(device);
        CleanupFramebuffer(device);
        FreeCommandBuffer(pool, device);

        CreateImage(device, viewInfo);
        CreateFrameBuffer(device, renderPass, swapchainExtent);
        CreateCommandBuffer(device, pool);
    }
}