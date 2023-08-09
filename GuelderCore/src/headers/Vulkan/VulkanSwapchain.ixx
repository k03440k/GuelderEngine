module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanSwapchain;

import :IVulkanObject;
import :VulkanQueueFamilyIndices;
import :VulkanCommandBuffer;
import :VulkanSwapchainFrame;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    struct VulkanSwapchainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };
    struct VulkanSwapchainCreateInfo
    {
        const vk::Device& device;
        const vk::PhysicalDevice& physicalDevice;
        const vk::SurfaceKHR& surface;
        const Types::uint& width;
        const Types::uint& height;
        const VulkanQueueFamilyIndices& queueFamilyIndices;
    };
    class VulkanSwapchain final : public IVulkanObject, INHERIT_GClass(VulkanSwapchain)
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanSwapchain);

        VulkanSwapchain(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
            const Types::uint& width, const Types::uint& height, const VulkanQueueFamilyIndices& queueFamilyIndices);
        VulkanSwapchain(const VulkanSwapchainCreateInfo& info);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void MakeFrames(const vk::Device& device, const vk::RenderPass& renderPass);

        //void RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const vk::RenderPass& renderPass, const Types::uint& imageIndex);
        void Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
            const Types::uint& width, const Types::uint& height, const VulkanQueueFamilyIndices& queueFamilyIndices);
    private:
        static vk::SurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
        static vk::PresentModeKHR ChooseSwapchainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
        static vk::Extent2D ChooseSwapchainExtent(const Types::uint& width, const Types::uint& height,
            const vk::SurfaceCapabilitiesKHR& capabilities);
        static VulkanSwapchainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);

        void Create(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
            const Types::uint& width, const Types::uint& height, const VulkanQueueFamilyIndices& queueFamilyIndices);

        friend class VulkanPipeline;

        VulkanSwapchainSupportDetails m_Details;
        VulkanCommandBuffer m_CommandBuffer;

        Types::uint m_MaxFramesInFlight;
        Types::uint m_CurrentFrameNumber;

        vk::SwapchainKHR m_Swapchain;
        std::vector<VulkanSwapchainFrame> m_Frames;
        vk::Format m_Format;
        vk::Extent2D m_Extent;
    };
}