module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VulkanPipeline;

import :IVulkanBase;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    class VulkanPipeline : INHERIT_GClass(VulkanPipeline), public IVulkanBase
    {
    public:
        VulkanPipeline() = default;
        /*
         *@param vertPath - path to the vertex shader file
         *@param fragPath - path to the fragment shader file
        */
        VulkanPipeline(
            const vk::Device& device,
            const vk::Extent2D& extent,
            const vk::Format& swapchainImageFormat,
            const std::string_view& vertexPath,
            const std::string_view& fragmentPath
        );
        ~VulkanPipeline() = default;

        VulkanPipeline(const VulkanPipeline& other);
        VulkanPipeline(VulkanPipeline&& other) noexcept;
        VulkanPipeline& operator=(const VulkanPipeline& other);
        VulkanPipeline& operator=(VulkanPipeline&& other) noexcept;

        void Cleanup(const vk::Device& device) const noexcept;
        virtual void Reset() noexcept override;
    private:
        static vk::PipelineLayout CreateLayout(const vk::Device& device);
        static vk::RenderPass CreateRenderPass(const vk::Device& device, const vk::Format& swapchainImageFormat);

        vk::RenderPass m_RenderPass;
        vk::PipelineLayout m_Layout;
        vk::Pipeline m_GraphicsPipeline;
    };
}