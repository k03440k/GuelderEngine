module;
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :VulkanSwapchainFrame;

import :VulkanSync;

namespace GuelderEngine::Vulkan
{
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
        sync = std::forward<VulkanSync>(other.sync);

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
        sync = std::forward<VulkanSync>(other.sync);

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
}