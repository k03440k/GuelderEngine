module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VulkanPipeline;

import :IVulkanBase;
import :VulkanSwapchain;
import GuelderEngine.Core.Types;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    class VulkanPipeline : INHERIT_GClass(VulkanPipeline), public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanPipeline);
        /*
         *@param vertPath - path to the vertex shader file
         *@param fragPath - path to the fragment shader file
        */
        VulkanPipeline(const VulkanSwapchainCreateInfo& swapchainInfo, const std::string_view& vertexPath, const std::string_view& fragmentPath);

        //VulkanPipeline(
        //    const vk::Device& device,
        //    /*const vk::Extent2D& extent,
        //    const vk::Format& swapchainImageFormat,*/
        //    const std::string_view& vertexPath,
        //    const std::string_view& fragmentPath
        //);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const Types::uint& imageIndex) const;
        void Render(const vk::Device& device) const;
    private:
        static vk::PipelineLayout CreateLayout(const vk::Device& device);
        static vk::RenderPass CreateRenderPass(const vk::Device& device, const vk::Format& swapchainImageFormat);
        static vk::Queue GetGraphicsQueue(const vk::Device& device, const VulkanQueueFamilyIndices& indices) noexcept;
        static vk::Queue GetPresentQueue(const vk::Device& device, const VulkanQueueFamilyIndices& indices) noexcept;

        vk::RenderPass m_RenderPass;
        vk::PipelineLayout m_Layout;
        vk::Pipeline m_GraphicsPipeline;

        struct
        {
            vk::Queue graphicsQueue;
            vk::Queue presentQueue;
        } m_Queues;

        VulkanSwapchain m_Swapchain;//DO smth with VulkanSwapchain it was in VulkanDeviceManager but now I confused
    };
}