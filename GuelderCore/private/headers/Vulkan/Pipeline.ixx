module;
#include <vulkan/vulkan.hpp>
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
    class Scene;

    class Pipeline : INHERIT_GClass(Pipeline), public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(Pipeline);
        Pipeline(
            const vk::Device& device, 
            const vk::PhysicalDevice& physicalDevice, 
            const vk::SurfaceKHR& surface,
            const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices, 
            const ShaderInfo& shaderInfo
        );

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void Render(
            const vk::Device& device, 
            const vk::PhysicalDevice& physicalDevice,
            const vk::SurfaceKHR& surface,
            const vk::Extent2D& extent,
            bool& wasWindowResized,
            const QueueFamilyIndices& queueFamilyIndices
        );

        /**
         * @brief Must be called before Render method
         */
        void SetMesh(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& queueFamilyIndices, const Mesh& mesh);
        void SetShaderInfo(const vk::Device& device, const ShaderInfo& shaderInfo);
    private:
        static vk::PipelineLayout CreateLayout(const vk::Device& device);
        static vk::RenderPass CreateRenderPass(const vk::Device& device, const vk::Format& swapchainImageFormat);

        static vk::Queue GetGraphicsQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept;
        static vk::Queue GetPresentQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept;
        static vk::Queue GetTransferQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept;

        void Create(const vk::Device& device, const ShaderInfo& shaderInfo);

        void Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices);

        void RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const Types::uint& imageIndex) const;

        ShaderManager m_ShaderManager;
        Swapchain m_Swapchain;

        Buffers::VertexBuffer m_VBuffer;
        Buffers::IndexBuffer m_IBuffer;

        vk::RenderPass m_RenderPass;
        vk::PipelineLayout m_Layout;
        vk::Pipeline m_GraphicsPipeline;

        struct
        {
            vk::Queue graphics;
            vk::Queue present;
            vk::Queue transfer;
        } m_Queues;
    };
}