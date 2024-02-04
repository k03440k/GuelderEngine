module;
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
    template<uint dimension>
    class RenderSystem : public IVulkanObject
    {
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

        /*template<class Iterator>
        requires std::is_base_of_v<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag> &&
        std::is_same_v<typename std::iterator_traits<Iterator>::value_type, RenderActor>
        void RenderActors(const vk::CommandBuffer& commandBuffer, const SimplePushConstantData& push, Iterator begin, Iterator end)
        {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline.GetPipeline());
            std::for_each(begin, end, [](const RenderActor& actor){ RenderActor(commandBuffer, push, actor.vertexBuffer, actor.indexBuffer)} );
        }*/
        void SetShaderInfo(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
        {
            m_Pipeline.SetShaderInfo(device, renderPass, shaderInfo);
        }
        
        void Render(const vk::CommandBuffer& commandBuffer, const SimplePushConstantData<dimension>& push, const Buffers::VertexBuffer& vertexBuffer, const Buffers::IndexBuffer& indexBuffer, const uint& verticesCount) const
        {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline.GetPipeline());

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
        Pipeline<dimension> m_Pipeline;
    };
}