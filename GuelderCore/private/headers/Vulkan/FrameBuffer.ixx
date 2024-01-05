module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:FrameBuffer;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct SwapchainFrame;
    struct SwapchainDepthImage;

    class FrameBuffer
    {
    public:
        DELETE_COPY_AND_MOVING(FrameBuffer);

        static void Make(
            const vk::Device& device,
            const vk::RenderPass& renderPass,
            const vk::Extent2D& swapchainExtent,
            std::vector<SwapchainFrame>& frames,
            const std::vector<SwapchainDepthImage>& depthImages
        );
    };
}