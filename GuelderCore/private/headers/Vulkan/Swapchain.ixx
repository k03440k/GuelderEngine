module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:Swapchain;

import :IVulkanObject;
import :QueueFamilyIndices;
import :CommandPool;
import :SwapchainFrame;
import :SwapchainDepthImage;
import :ShaderManager;
import GuelderEngine.Core.Types;

import <functional>;

export namespace GuelderEngine::Vulkan
{
    class Swapchain final : public IVulkanObject//, INHERIT_GClass(Swapchain)
    {
    public:
        //DECLARE_DEFAULT_CTOR_AND_DTOR_AND_COPYING_AND_MOVING(Swapchain);
        DECLARE_DEFAULT_CTOR_AND_DTOR(Swapchain);
        DECLARE_MOVING(Swapchain);

        Swapchain(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::SurfaceKHR& surface,
            const vk::SurfaceCapabilitiesKHR& surfaceCapabilities,
            const vk::SurfaceFormatKHR& surfaceFormat,
            const vk::PresentModeKHR& presentMode,
            const vk::Extent2D& extent,
            const vk::CommandPool& commandPool,
            const QueueFamilyIndices& queueFamilyIndices
        );

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device, const vk::CommandPool& commandPool) const noexcept;

        /**
         *@brief Optimized recreation
        */
        void Recreate
        (
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::SurfaceKHR& surface,
            const vk::SurfaceCapabilitiesKHR& surfaceCapabilities,
            const vk::SurfaceFormatKHR& surfaceFormat,
            const vk::PresentModeKHR& presentMode,
            const vk::Extent2D& extent,
            const vk::CommandPool& commandPool,
            const QueueFamilyIndices& queueFamilyIndices
        );

        bool CompareSwapchainFormats(const Swapchain& other) const noexcept;

        const SwapchainFrame& GetCurrentFrame() const;
        std::vector<vk::Image> GetSwapchainImages(const vk::Device& device) const;
		const std::vector<SwapchainDepthImage>& GetDepthImages() const;
        const std::vector<SwapchainFrame>& GetFrames() const;
		
        const vk::SwapchainKHR& GetSwapchain() const noexcept;
        const vk::Format& GetFormat() const noexcept;
        const vk::Format& GetDepthFormat() const noexcept;
        const vk::Extent2D& GetExtent2D() const noexcept;
        uint GetMaxFramesInFlight() const noexcept;
        const vk::RenderPass& GetRenderPass() const noexcept;
        float GetAspectRatio() const;
        uint GetMinImageCount() const;

        uint currentFrameNumber;

    private:
        static vk::Extent2D ChooseExtent(const vk::Extent2D& extent, const vk::SurfaceCapabilitiesKHR& capabilities);

        static vk::RenderPass CreateRenderPass(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::Format& swapchainImageFormat);

        void Create
        (
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::SurfaceKHR& surface,
            const vk::SurfaceCapabilitiesKHR& surfaceCapabilities,
            const vk::SurfaceFormatKHR& surfaceFormat,
            const vk::PresentModeKHR& presentMode,
            const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices
        );
        /**
         *@brief For ctor
        */
        void CreateFrames(const vk::Device& device, const vk::Format& format, const vk::CommandPool& commandPool, const std::vector<vk::Image>& images);
        void CreateDepthImages(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::Format& format, const QueueFamilyIndices& queueFamilyIndices, const std::vector<vk::Image>& images);

        //if the swapchain was created
        bool m_IsSwapchain : 1;

        uint m_MinImageCount;
        uint m_MaxFramesInFlight;

        vk::RenderPass m_RenderPass;
        vk::SwapchainKHR m_Swapchain;
        std::vector<SwapchainFrame> m_Frames;
        std::vector<SwapchainDepthImage> m_DepthImages;
        vk::Format m_Format;
        vk::Format m_DepthFormat;
        vk::Extent2D m_Extent;
    };
}