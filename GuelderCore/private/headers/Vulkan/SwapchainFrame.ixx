module;
#include <compare>
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:SwapchainFrame;

import :IVulkanObject;
import :FrameContext;
import :FrameSync;

import <limits>;

export namespace GuelderEngine::Vulkan
{
    struct SwapchainFrame final : public FrameContext
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(SwapchainFrame);
        DECLARE_DEFAULT_COPYING(SwapchainFrame);
        DECLARE_MOVING(SwapchainFrame);

        SwapchainFrame(
            const vk::Device& device,
            const vk::CommandPool& commandPool,
            const vk::RenderPass& renderPass,
            const vk::ImageViewCreateInfo& viewInfo,
            const vk::ImageView& depthImageView,
            const vk::Extent2D& extent,
            const uint& layers = 1
        );

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device, const vk::CommandPool& commandPool) const noexcept override;

        void Recreate(
            const vk::Device& device,
            const vk::RenderPass& renderPass,
            const vk::ImageViewCreateInfo& viewInfo,
            const vk::ImageView& depthImageView,
            const vk::CommandPool& commandPool,
            const vk::Extent2D& swapchainExtent
        );

        void WaitForFence(const vk::Device& device, const uint64_t& delay = std::numeric_limits<uint64_t>::max()) const;
        void ResetFence(const vk::Device& device) const;

        vk::Image image;
        vk::ImageView imageView;
        FrameSync sync;
    private:
        using FrameContext::Recreate;
    };
}