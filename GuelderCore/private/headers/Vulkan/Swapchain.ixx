module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:Swapchain;

import :IVulkanObject;
import :QueueFamilyIndices;
import :CommandPool;
import :SwapchainFrame;
import :ShaderManager;
import GuelderEngine.Core.Types;

import <functional>;

export namespace GuelderEngine::Vulkan
{
    struct SwapchainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };
    class Swapchain final : public IVulkanObject//, INHERIT_GClass(Swapchain)
    {
    public:
        DECLARE_DCAD_AND_CAM(Swapchain);

        Swapchain(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
            const vk::Extent2D& extent, const QueueFamilyIndices& queueFamilyIndices);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void MakeFrames(const vk::Device& device, const vk::RenderPass& renderPass);
        
        /**
         *@brief Optimized recreation
        */
        void Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::RenderPass& renderPass, const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices);

        const CommandPool& GetCommandPool() const noexcept;
        const CommandPool& GetCommandPoolTransfer() const noexcept;

        Types::uint IncrementCurrentFrame() noexcept;

        const std::vector<SwapchainFrame>& GetFrames() const noexcept;
        const vk::SwapchainKHR& GetSwapchain() const noexcept;
        const vk::Format& GetFormat() const noexcept;
        const vk::Extent2D& GetExtent2D() const noexcept;
        Types::uint GetCurrentFrameNumber() const noexcept;
        Types::uint GetMaxFramesInFlight() const noexcept;

    private:
        static vk::SurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
        static vk::PresentModeKHR ChooseSwapchainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
        static vk::Extent2D ChooseSwapchainExtent(const vk::Extent2D& extent, const vk::SurfaceCapabilitiesKHR& capabilities);
        static SwapchainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);

        void Create(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices);
        /**
         *@brief For ctor
        */
        void CreateFrames(const vk::Device& device, const vk::Format& format, const std::vector<vk::Image>& images);

        ShaderManager m_ShaderManager;
        SwapchainSupportDetails m_Details;
        CommandPool m_CommandPool;
        CommandPool m_CommandPoolTransfer;

        //if the swapchain was created
        bool m_IsSwapchain : 1;

        Types::uint m_MaxFramesInFlight;
        Types::uint m_CurrentFrameNumber;

        vk::SwapchainKHR m_Swapchain;
        std::vector<SwapchainFrame> m_Frames;
        vk::Format m_Format;
        vk::Extent2D m_Extent;
    };
}