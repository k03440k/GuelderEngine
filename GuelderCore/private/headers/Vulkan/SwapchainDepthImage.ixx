module;
#include <vulkan/vulkan.hpp>
#include "../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:SwapchainDepthImage;

import :IVulkanObject;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    struct SwapchainDepthImage final : public IVulkanObject
    {
    public:
        SwapchainDepthImage() = default;
        ~SwapchainDepthImage() override = default;
        DECLARE_DEFAULT_COPY(SwapchainDepthImage);
        DECLARE_MOVING(SwapchainDepthImage);

        SwapchainDepthImage(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::Extent2D& swapchainExtent,
            const vk::Format& depthFormat, 
            const vk::SharingMode& sharingMode,
            const uint& queueFamilyIndexCount,
            const uint* queueFamilyIndices
        );

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        //mb temp
        vk::Image image;
        vk::ImageView imageView;
        vk::DeviceMemory memory;
    private:
    };
}