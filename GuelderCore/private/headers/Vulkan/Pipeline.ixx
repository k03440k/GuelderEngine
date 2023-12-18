module;
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:Pipeline;

import :IVulkanObject;
import :Swapchain;
import :ShaderManager;
import :Mesh;
import :ShaderManager;
import :VertexBuffer;
import :IndexBuffer;
import GuelderEngine.Core.Types;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct QueueFamilyIndices;

    class Pipeline : /*INHERIT_GClass(Pipeline),*/ public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(Pipeline);
        DECLARE_MOVING(Pipeline);
        DECLARE_DEFAULT_COPY(Pipeline);
        Pipeline(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        /*void Render(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::SurfaceKHR& surface,
            const vk::RenderPass& renderPass,
            const vk::Extent2D& extent,
            bool& wasWindowResized,
            const QueueFamilyIndices& queueFamilyIndices,
            const Vulkan::Buffers::VertexBuffer& vertexBuffers,
            const Vulkan::Buffers::IndexBuffer& indexBuffer,
            const SimplePushConstantData& push
        );*/

        /**
         * @brief Must be called before Render method
         */
        //void SetMesh(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& queueFamilyIndices, const Mesh2D& mesh);
        void SetShaderInfo(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo);

        const vk::Pipeline& GetPipeline() const noexcept;
        const vk::PipelineLayout& GetPipelineLayout() const noexcept;
        const ShaderManager& GetShaderManager() const;

    private:
        static vk::PipelineLayout CreateLayout(const vk::Device& device);

        void Create(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo);

        void Recreate(const vk::Device& device, const vk::RenderPass& renderPass);

        /*void RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const vk::RenderPass& renderPass, const uint& imageIndex, const Buffers::VertexBuffer& vertexBuffers,
            const Buffers::IndexBuffer& indexBuffers, uint currentFrameNumber, const SimplePushConstantData& push) const;*/

        ShaderManager m_ShaderManager;

        vk::PipelineLayout m_Layout;
        vk::Pipeline m_GraphicsPipeline;
    };
}