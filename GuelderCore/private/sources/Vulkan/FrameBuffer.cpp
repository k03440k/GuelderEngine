module;
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :FrameBuffer;

import :SwapchainFrame;
import :SwapchainDepthImage;

import <vector>;
import <array>;

namespace GuelderEngine::Vulkan
{
    void FrameBuffer::Make(
        const vk::Device& device,
        const vk::RenderPass& renderPass,
        const vk::Extent2D& swapchainExtent, 
        std::vector<SwapchainFrame>& frames,
        const std::vector<SwapchainDepthImage>& depthImages
    )
    {
        for(uint i = 0; i < frames.size(); ++i)
        {
            std::array attachments{frames[i].imageView, depthImages[i].imageView};//idk

            //auto attachment = frames[i].imageView;

            const vk::FramebufferCreateInfo framebufferInfo(
                vk::FramebufferCreateFlags(),
                renderPass,
                attachments.size(),
                attachments.data(),
                swapchainExtent.width,
                swapchainExtent.height,
                1
            );

            frames[i].framebuffer = device.createFramebuffer(framebufferInfo);
            //frames[i].CreateFrameBuffer(device, framebufferInfo);
        }
    }
}