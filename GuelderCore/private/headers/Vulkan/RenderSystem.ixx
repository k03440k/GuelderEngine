module;
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>
#include "../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:RenderSystem;

import :IVulkanObject;
import :Mesh;
import :VertexBuffer;
import :IndexBuffer;
import :StagingBuffer;
import :Pipeline;

export namespace GuelderEngine::Vulkan
{
    template<uint dimension, uint matDimension>
    class RenderSystem {};
    template<>
    class RenderSystem<3, 4> : public IVulkanObject
    {
    public:
        using PushData = Vulkan::SimplePushConstantData<3>;
    public:
        DELETE_COPYING_AND_MOVING(RenderSystem);
        DECLARE_DEFAULT_CTOR_AND_DTOR(RenderSystem);

        RenderSystem(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
            : m_Pipeline(device, renderPass, shaderInfo) {}

        void Reset() noexcept override
        {
            m_Pipeline.Reset();
        }
        void Cleanup(const vk::Device& device) const noexcept
        {
            m_Pipeline.Cleanup(device);
        }

        void SetShaderInfo(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
        {
            m_Pipeline.SetShaderInfo(device, renderPass, shaderInfo);
        }

        void Render(
            const vk::CommandBuffer& commandBuffer,
            const Buffers::VertexBuffer& vertexBuffer,
            const Buffers::IndexBuffer& indexBuffer,
            const uint& verticesCount,
            const Mat<4>& actorTransform,
            const Mat<4>& cameraProjection
        ) const
        {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline.GetPipeline());

            const PushData push(cameraProjection * actorTransform);

            vertexBuffer.Bind(commandBuffer, { 0 });
            if(indexBuffer.GetIndicesCount())
                indexBuffer.Bind(commandBuffer, { 0 });
            commandBuffer.pushConstants(m_Pipeline.GetPipelineLayout(), vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(push), &push);

            if(indexBuffer.GetIndicesCount())
                commandBuffer.drawIndexed(indexBuffer.GetIndicesCount(), 1, 0, 0, 0);
            else
                commandBuffer.draw(verticesCount, 1, 0, 0);
        }
    private:
        Pipeline<3> m_Pipeline;
    };
    template<>
    class RenderSystem<2, 2> : public IVulkanObject
    {
    public:
        using PushData = Vulkan::SimplePushConstantData<2>;
    public:
        DELETE_COPYING_AND_MOVING(RenderSystem);
        DECLARE_DEFAULT_CTOR_AND_DTOR(RenderSystem);

        RenderSystem(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
            : m_Pipeline(device, renderPass, shaderInfo) {}

        void Reset() noexcept override
        {
            m_Pipeline.Reset();
        }
        void Cleanup(const vk::Device& device) const noexcept
        {
            m_Pipeline.Cleanup(device);
        }

        void SetShaderInfo(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
        {
            m_Pipeline.SetShaderInfo(device, renderPass, shaderInfo);
        }

        void Render(
            const vk::CommandBuffer& commandBuffer,
            const Buffers::VertexBuffer& vertexBuffer,
            const Buffers::IndexBuffer& indexBuffer,
            const uint& verticesCount,
            const Mat<2>& actorTransform
        ) const
        {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline.GetPipeline());

            const PushData push(actorTransform);

            vertexBuffer.Bind(commandBuffer, { 0 });
            if(indexBuffer.GetIndicesCount())
                indexBuffer.Bind(commandBuffer, { 0 });
            commandBuffer.pushConstants(m_Pipeline.GetPipelineLayout(), vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(push), &push);

            if(indexBuffer.GetIndicesCount())
                commandBuffer.drawIndexed(indexBuffer.GetIndicesCount(), 1, 0, 0, 0);
            else
                commandBuffer.draw(verticesCount, 1, 0, 0);
        }
    private:
        Pipeline<2> m_Pipeline;
    };
}