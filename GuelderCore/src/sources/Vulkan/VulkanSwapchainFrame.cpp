module;
#include <vulkan/vulkan.hpp>

#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :VulkanSwapchainFrame;

import :VulkanSync;
import :VulkanCommandBuffer;

namespace GuelderEngine::Vulkan
{
    VulkanSwapchainFrame::VulkanSwapchainFrame(const vk::Device& device, const vk::Image& image, const vk::ImageViewCreateInfo& viewInfo)
    {
        CreateImage(device, image, viewInfo);
        sync = VulkanSwapchainFrameSync(device);
        //don't allocate command buffer, because we don't have actual access to vk::CommandPool
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
    void VulkanSwapchainFrame::Reset() noexcept
    {
        image = nullptr;
        imageView = nullptr;
        framebuffer = nullptr;
        commandBuffer = nullptr;
        sync.Reset();
    }
    void VulkanSwapchainFrame::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyImageView(imageView);
        device.destroyFramebuffer(framebuffer);
        sync.Cleanup(device);
    }
    void VulkanSwapchainFrame::CreateFrameBuffer(const vk::Device& device, const vk::FramebufferCreateInfo& info)
    {
        framebuffer = device.createFramebuffer(info);
    }
    void VulkanSwapchainFrame::WaitForImage(const vk::Device& device, const uint64_t& delay) const
    {
        GE_CORE_CLASS_ASSERT(device.waitForFences(1, &sync.m_InFlightFence, VK_TRUE, delay) == vk::Result::eSuccess, "cannot wait for fence");
    }
    void VulkanSwapchainFrame::ResetFence(const vk::Device& device) const
    {
        GE_CORE_CLASS_ASSERT(device.resetFences(1, &sync.m_InFlightFence) == vk::Result::eSuccess, "cannot reset fence");
    }
    void VulkanSwapchainFrame::CreateImage(const vk::Device& device, const vk::Image& image, const vk::ImageViewCreateInfo& viewInfo)
    {
        this->image = image;
        imageView = device.createImageView(viewInfo);
    }
}