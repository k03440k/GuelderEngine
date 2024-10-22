module;
#include <compare>
#include <vulkan/vulkan.hpp>
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :SwapchainFrame;

import GuelderEngine.Core.Types;
import :FrameContext;
import :FrameSync;
import :CommandPool;

import <array>;

namespace GuelderEngine::Vulkan
{
    SwapchainFrame::SwapchainFrame(
        const vk::Device& device,
        const vk::CommandPool& commandPool,
        const vk::RenderPass& renderPass,
        const vk::ImageViewCreateInfo& viewInfo,
        const vk::ImageView& depthImageView,
        const vk::Extent2D& extent,
        const uint& layers
    )
        : image(viewInfo.image), imageView(device.createImageView(viewInfo))//, FrameContext(device, commandPool, renderPass, {imageView, depthImageView}, extent, layers)
    {
        commandBuffer = CreateCommandBuffer(device, commandPool);

        std::vector attachments{ imageView, depthImageView };

        const vk::FramebufferCreateInfo framebufferInfo(
            vk::FramebufferCreateFlags(),
            renderPass,
            attachments.size(),
            attachments.data(),
            extent.width,
            extent.height,
            1
        );

        frameBuffer = device.createFramebuffer(framebufferInfo);
        sync = FrameSync(device);
    }
    SwapchainFrame::SwapchainFrame(SwapchainFrame&& other) noexcept
        : FrameContext(std::move(other))
    {
        image = other.image;
        imageView = other.imageView;
        sync = std::move(other.sync);

        other.Reset();
    }
    SwapchainFrame& SwapchainFrame::operator=(SwapchainFrame&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        sync = std::move(other.sync);
        FrameContext::operator=(std::move(other));

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void SwapchainFrame::Reset() noexcept
    {
        image = nullptr;
        imageView = nullptr;
        sync.Reset();
        FrameContext::Reset();
    }
    void SwapchainFrame::Cleanup(const vk::Device& device, const vk::CommandPool& commandPool) const noexcept
    {
        device.destroyImageView(imageView);
        sync.Cleanup(device);
        FrameContext::Cleanup(device, commandPool);
    }
    void SwapchainFrame::WaitForFence(const vk::Device& device, const uint64_t& delay) const
    {
        GE_ASSERT(device.waitForFences(1, &sync.GetFlightFence(), VK_TRUE, delay) == vk::Result::eSuccess, "cannot wait for fence");
    }
    void SwapchainFrame::ResetFence(const vk::Device& device) const
    {
        GE_ASSERT(device.resetFences(1, &sync.GetFlightFence()) == vk::Result::eSuccess, "cannot reset fence");
    }
    void SwapchainFrame::Recreate(
        const vk::Device& device,
        const vk::RenderPass& renderPass,
        const vk::ImageViewCreateInfo& viewInfo,
        const vk::ImageView& depthImageView,
        const vk::CommandPool& commandPool,
        const vk::Extent2D& swapchainExtent
    )
    {
        device.destroyImageView(imageView);

        image = viewInfo.image;
        imageView = device.createImageView(viewInfo);

        FrameContext::Recreate(device, renderPass, { imageView, depthImageView }, commandPool, swapchainExtent);
    }
}