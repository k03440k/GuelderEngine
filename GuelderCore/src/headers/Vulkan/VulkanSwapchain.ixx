module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanSwapchain;

import :IVulkanBase;
import GuelderEngine.Core.Types;

import <optional>;

export namespace GuelderEngine::Vulkan
{
    struct QueueFamilyIndices : INHERIT_GClass(QueueFamilyIndices)
    {
        QueueFamilyIndices() = default;
        QueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
        ~QueueFamilyIndices() = default;

        QueueFamilyIndices(const QueueFamilyIndices& other);
        QueueFamilyIndices& operator=(const QueueFamilyIndices& other);

        //location of graphics Queue Family
        std::optional<Types::uint> graphicsFamily;
        std::optional<Types::uint> presentFamily;

        bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };
    struct SwapChainFrame
    {
        SwapChainFrame() = default;
        SwapChainFrame(const SwapChainFrame& other);
        SwapChainFrame& operator=(const SwapChainFrame& other);

        vk::Image image;
        vk::ImageView imageView;
    };
}

export namespace GuelderEngine::Vulkan
{
    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };
    class VulkanSwapchain final : public IVulkanBase, INHERIT_GClass(VulkanSwapchain)
    {
    public:
        VulkanSwapchain() = default;
        VulkanSwapchain(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
            const Types::uint& width, const Types::uint& height, const QueueFamilyIndices& queueFamilyIndices);
        ~VulkanSwapchain() = default;

        VulkanSwapchain(const VulkanSwapchain& other);
        VulkanSwapchain(VulkanSwapchain&& other) noexcept;
        VulkanSwapchain& operator=(const VulkanSwapchain& other);
        VulkanSwapchain& operator=(VulkanSwapchain&& other) noexcept;

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;
    private:
        static vk::SurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
        static vk::PresentModeKHR ChooseSwapchainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
        static vk::Extent2D ChooseSwapchainExtent(const Types::uint& width, const Types::uint& height,
            const vk::SurfaceCapabilitiesKHR& capabilities);
        static SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);

        friend class VulkanDeviceManager;

        SwapChainSupportDetails m_Details;

        vk::SwapchainKHR m_Swapchain;
        std::vector<SwapChainFrame> m_Frames;
        vk::Format m_Format;
        vk::Extent2D m_Extent;
    };
}