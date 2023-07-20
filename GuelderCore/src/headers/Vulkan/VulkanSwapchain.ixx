module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanSwapchain;

import :IVulkanBase;
import :VulkanSync;
import GuelderEngine.Core.Types;

import <optional>;

export namespace GuelderEngine::Vulkan
{
    struct VulkanQueueFamilyIndices : INHERIT_GClass(VulkanQueueFamilyIndices)
    {
        DECLARE_DEFAULT_CTOR_AND_DTOR(VulkanQueueFamilyIndices);
        VulkanQueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

        VulkanQueueFamilyIndices(const VulkanQueueFamilyIndices& other);
        VulkanQueueFamilyIndices& operator=(const VulkanQueueFamilyIndices& other);

        //location of graphics Queue Family
        std::optional<Types::uint> graphicsFamily;
        std::optional<Types::uint> presentFamily;

        bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };
    struct VulkanSwapchainFrame : public IVulkanObject
    {
        DECLARE_DCAD_AND_CAM(VulkanSwapchainFrame);

        virtual void Reset() noexcept override;

        vk::Image image;
        vk::ImageView imageView;
        vk::Framebuffer framebuffer;
        vk::CommandBuffer commandBuffer;
    };
    class VulkanFrameBuffer : INHERIT_GClass(VulkanFrameBuffer), public IVulkanObject
    {
    public:
        DELETE_COPY_AND_MOVING(VulkanFrameBuffer);

        static void Make(
            const vk::Device& device,
            const vk::RenderPass& renderPass,
            const vk::Extent2D& swapchainExtent,
            std::vector<VulkanSwapchainFrame>& frames
        );
    };
    class VulkanCommandBuffer : INHERIT_GClass(VulkanCommandBuffer), public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanCommandBuffer);

        VulkanCommandBuffer(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, const vk::SurfaceKHR& surface, std::vector<VulkanSwapchainFrame>& frames);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;
    private:
        static vk::CommandPool MakePool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, const vk::SurfaceKHR& surface);
        static vk::CommandBuffer MakeBuffer(const vk::Device& device, const vk::CommandPool& pool, std::vector<VulkanSwapchainFrame>& frames);

        vk::CommandPool m_CommandPool;
        vk::CommandBuffer m_CommandBuffer;
    };
}

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
    private:
        static vk::SurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
        static vk::PresentModeKHR ChooseSwapchainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
        static vk::Extent2D ChooseSwapchainExtent(const Types::uint& width, const Types::uint& height,
            const vk::SurfaceCapabilitiesKHR& capabilities);
        static VulkanSwapchainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);

        friend class VulkanPipeline;

        VulkanSwapchainSupportDetails m_Details;
        VulkanCommandBuffer m_CommandBuffer;
        VulkanSync m_Sync;

        vk::SwapchainKHR m_Swapchain;
        std::vector<VulkanSwapchainFrame> m_Frames;
        vk::Format m_Format;
        vk::Extent2D m_Extent;
    };
}