module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanSwapchain;

import :IVulkanObject;
import :VulkanQueueFamilyIndices;
import :VulkanCommandPool;
import :VulkanSwapchainFrame;
import :VulkanShaderManager;
import GuelderEngine.Core.Types;

import <functional>;

export namespace GuelderEngine::Vulkan
{
    struct VulkanSwapchainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };
    class VulkanSwapchain final : public IVulkanObject, INHERIT_GClass(VulkanSwapchain)
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanSwapchain);

        VulkanSwapchain(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
            const vk::Extent2D& extent, const VulkanQueueFamilyIndices& queueFamilyIndices);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void MakeFrames(const vk::Device& device, const vk::RenderPass& renderPass);
        
        /*
         *@brief Certain cleanup before recreation. Must be called after cleanups of vk::RenderPass'es, vk::Pipeline's
        */
        void Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::RenderPass& renderPass, const vk::Extent2D& extent,
            const VulkanQueueFamilyIndices& queueFamilyIndices);
    private:
        static vk::SurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
        static vk::PresentModeKHR ChooseSwapchainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
        static vk::Extent2D ChooseSwapchainExtent(const vk::Extent2D& extent, const vk::SurfaceCapabilitiesKHR& capabilities);
        static VulkanSwapchainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);

        void Create(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
            const VulkanQueueFamilyIndices& queueFamilyIndices);
        /*
         *@brief For ctor
        */
        void CreateFrames(const vk::Device& device, const vk::Format& format, const std::vector<vk::Image>& images);

        friend class VulkanPipeline;

        VulkanShaderManager m_ShaderManager;
        VulkanSwapchainSupportDetails m_Details;
        VulkanCommandPool m_CommandPool;

        Types::uint m_MaxFramesInFlight;
        Types::uint m_CurrentFrameNumber;

        vk::SwapchainKHR m_Swapchain;
        std::vector<VulkanSwapchainFrame> m_Frames;
        vk::Format m_Format;
        vk::Extent2D m_Extent;
    };
}