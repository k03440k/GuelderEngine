module;
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:Pipeline;

import :ShaderManager;
import :Swapchain;
import :Mesh;

import :VertexBuffer;
import :IndexBuffer;
import :StagingBuffer;

import GuelderEngine.Utils;
import GuelderEngine.Core.Types;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct QueueFamilyIndices;

    template<uint dimension>
    class Pipeline : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(Pipeline);
        DECLARE_DEFAULT_COPYING(Pipeline);

        Pipeline(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
        {
            m_ShaderManager = ShaderManager(shaderInfo);

            Create(device, renderPass, shaderInfo);
        }
        Pipeline(Pipeline&& other) noexcept
        {
            m_GraphicsPipeline = other.m_GraphicsPipeline;
            m_Layout = other.m_Layout;
            m_ShaderManager = std::forward<ShaderManager>(other.m_ShaderManager);

            other.Reset();
        }
        Pipeline& operator=(Pipeline&& other) noexcept
        {
            m_GraphicsPipeline = other.m_GraphicsPipeline;
            m_Layout = other.m_Layout;
            m_ShaderManager = std::forward<ShaderManager>(other.m_ShaderManager);

            other.Reset();

            return *this;
        }

        void Reset() noexcept override
        {
            m_GraphicsPipeline = nullptr;
            m_Layout = nullptr;
        }
        void Cleanup(const vk::Device& device) const noexcept
        {
            device.destroyPipelineLayout(m_Layout);
            device.destroyPipeline(m_GraphicsPipeline);
        }

        /**
         * @brief Must be called before Render method
         */
        void SetShaderInfo(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
        {
            m_ShaderManager = ShaderManager(shaderInfo);

            Create(device, renderPass, shaderInfo);
        }

        const vk::Pipeline& GetPipeline() const noexcept
        {
            return m_GraphicsPipeline;
        }
        const vk::PipelineLayout& GetPipelineLayout() const noexcept
        {
            return m_Layout;
        }
        const ShaderManager& GetShaderManager() const
        {
            return m_ShaderManager;
        }

    private:
        template<uint _dimension>
        static vk::PipelineLayout CreateLayout(const vk::Device& device)
        {
            const vk::PushConstantRange constantInfo(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(SimplePushConstantData<_dimension>));
            const vk::PipelineLayoutCreateInfo layoutInfo(
                vk::PipelineLayoutCreateFlags(),
                0,
                nullptr,
                1,
                &constantInfo
            );

            return device.createPipelineLayout(layoutInfo);
        }

        void Create(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
        {
            const auto bindingDescription = Vertex<dimension>::GetBindingDescription();
            const auto attributeDescriptions = Vertex<dimension>::GetAttributeDescriptions(shaderInfo.info);

            const vk::PipelineVertexInputStateCreateInfo vertexInfo{ vk::PipelineVertexInputStateCreateFlags(),
                1,
                &bindingDescription,
                attributeDescriptions.size(),
                attributeDescriptions.data()
            };
            const vk::PipelineInputAssemblyStateCreateInfo assemblyInfo{vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList, VK_FALSE};

            std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
            shaderStages.reserve(2);

            //vertex shader
            const vk::ShaderModule vertex = ShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(shaderInfo.vertexPath), device);
            const vk::PipelineShaderStageCreateInfo vertexShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertex, "main");
            shaderStages.push_back(vertexShaderInfo);
            //fragment shader
            const vk::ShaderModule fragment = ShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(shaderInfo.fragmentPath), device);
            const vk::PipelineShaderStageCreateInfo fragmentShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragment, "main");
            shaderStages.push_back(fragmentShaderInfo);

            vk::PipelineViewportStateCreateInfo viewportInfo{
                vk::PipelineViewportStateCreateFlags(),
                1,          //viewports
                nullptr,    //viewports
                1,          //scissors
                nullptr     //scissors
            };

            //rasterizer
            const vk::PipelineRasterizationStateCreateInfo rasterizationInfo{
                vk::PipelineRasterizationStateCreateFlags(),
                VK_FALSE,
                VK_FALSE,
                vk::PolygonMode::eFill,
                vk::CullModeFlagBits::eNone,//changed from eBack
                vk::FrontFace::eClockwise,
                VK_FALSE,
                0.f, 0.f, 0.f, 1.0f
            };

            //multisampling
            const vk::PipelineMultisampleStateCreateInfo multisampleInfo{
                vk::PipelineMultisampleStateCreateFlags(),
                vk::SampleCountFlagBits::e1,
                VK_FALSE
            };

            //color blending
            vk::PipelineColorBlendAttachmentState colorBlendAttachment;
            colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
            colorBlendAttachment.blendEnable = VK_FALSE;//changed from VK_TRUE
            colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
            colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
            colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
            colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
            colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
            colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
            const vk::PipelineColorBlendStateCreateInfo colorBlendInfo{
                vk::PipelineColorBlendStateCreateFlags(),
                VK_FALSE,
                vk::LogicOp::eCopy,
                1,
                &colorBlendAttachment,
                { 0, 0, 0, 0 }
            };

            const vk::PipelineDepthStencilStateCreateInfo stencilInfo{
                vk::PipelineDepthStencilStateCreateFlagBits(),
                VK_TRUE,
                VK_TRUE,
                vk::CompareOp::eLess,
                VK_FALSE,
                VK_FALSE,
                {},
                {},
                0.0f,
                1.0f
            };

            m_Layout = CreateLayout<dimension>(device);

            const std::vector<vk::DynamicState> dynamicStateEnables{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };
            vk::PipelineDynamicStateCreateInfo dynamicStateInfo{};
            dynamicStateInfo.pDynamicStates = dynamicStateEnables.data();
            dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

            const vk::GraphicsPipelineCreateInfo createInfo(
                vk::PipelineCreateFlagBits(),
                shaderStages.size(),
                shaderStages.data(),
                &vertexInfo,
                &assemblyInfo,
                nullptr,
                &viewportInfo,
                &rasterizationInfo,
                &multisampleInfo,
                &stencilInfo,
                &colorBlendInfo,
                &dynamicStateInfo,
                m_Layout,
                renderPass,
                0,
                nullptr
            );

            m_GraphicsPipeline = device.createGraphicsPipeline(nullptr, createInfo).value;

            device.destroyShaderModule(vertex);
            device.destroyShaderModule(fragment);
        }

        void Recreate(const vk::Device& device, const vk::RenderPass& renderPass)
        {
            device.waitIdle();

            device.destroyPipelineLayout(m_Layout);
            device.destroyPipeline(m_GraphicsPipeline);

            Create(device, renderPass, m_ShaderManager.GetShaderInfo());
        }

        ShaderManager m_ShaderManager;

        vk::PipelineLayout m_Layout;
        vk::Pipeline m_GraphicsPipeline;
    };
}