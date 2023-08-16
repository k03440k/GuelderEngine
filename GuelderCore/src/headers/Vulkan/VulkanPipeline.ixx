module;
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VulkanPipeline;

import :IVulkanObject;
import :VulkanSwapchain;
import :VulkanShaderManager;
import GuelderEngine.Core.Types;

//import GuelderEngine.Core;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct VulkanQueueFamilyIndices;
    class VulkanScene;

    class VulkanPipeline : INHERIT_GClass(VulkanPipeline), public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanPipeline);
        /*
         *@param vertPath - path to the vertex shader file
         *@param fragPath - path to the fragment shader file
        */
        VulkanPipeline(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
            const Types::uint& width, const Types::uint& height, const VulkanQueueFamilyIndices& queueFamilyIndices, const std::string_view& vertexPath, const std::string_view& fragmentPath);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void Render(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
            const VulkanQueueFamilyIndices& queueFamilyIndices, const VulkanScene& scene);

    private:
        static vk::PipelineLayout CreateLayout(const vk::Device& device);
        static vk::RenderPass CreateRenderPass(const vk::Device& device, const vk::Format& swapchainImageFormat);
        static vk::Queue GetGraphicsQueue(const vk::Device& device, const VulkanQueueFamilyIndices& indices) noexcept;
        static vk::Queue GetPresentQueue(const vk::Device& device, const VulkanQueueFamilyIndices& indices) noexcept;

        void Create(const vk::Device& device, const std::string_view& vertexPath, const std::string_view& fragmentPath);

        void Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
            const VulkanQueueFamilyIndices& queueFamilyIndices);

        void RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const Types::uint& imageIndex, const VulkanScene& scene) const;

        VulkanShaderManager m_ShaderManager;
        VulkanSwapchain m_Swapchain;

        vk::RenderPass m_RenderPass;
        vk::PipelineLayout m_Layout;
        vk::Pipeline m_GraphicsPipeline;

        struct
        {
            vk::Queue graphicsQueue;
            vk::Queue presentQueue;
        } m_Queues;

    };
}