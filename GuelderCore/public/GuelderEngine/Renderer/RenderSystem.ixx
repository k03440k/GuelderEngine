//TODO: move this file to vulkan folder
module;
#include <vulkan/vulkan.hpp>
#include "../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:RenderSystem;

import :IVulkanObject;
import :Pipeline;
import :Mesh;
import :VertexBuffer;
import :IndexBuffer;

import <vector>;
import <type_traits>;
import <algorithm>;

export namespace GuelderEngine::Vulkan
{
    class RenderSystem : public IVulkanObject
    {
    public:
        DELETE_COPY_AND_MOVING(RenderSystem);
        DECLARE_DEFAULT_CTOR_AND_DTOR(RenderSystem);

        RenderSystem(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        /*template<class Iterator>
        requires std::is_base_of_v<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag> &&
        std::is_same_v<typename std::iterator_traits<Iterator>::value_type, RenderActor>
        void RenderActors(const vk::CommandBuffer& commandBuffer, const SimplePushConstantData& push, Iterator begin, Iterator end)
        {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline.GetPipeline());
            std::for_each(begin, end, [](const RenderActor& actor){ RenderActor(commandBuffer, push, actor.vertexBuffer, actor.indexBuffer)} );
        }*/
        void SetShaderInfo(const vk::Device& device, const vk::RenderPass& renderPass, const Vulkan::ShaderInfo& shaderInfo);

        void Render(const vk::CommandBuffer& commandBuffer, const SimplePushConstantData& push, const Buffers::VertexBuffer& vertexBuffer, const Buffers::IndexBuffer& indexBuffer);
    private:
        Pipeline m_Pipeline;
    };
}