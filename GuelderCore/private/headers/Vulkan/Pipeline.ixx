module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:Pipeline;

import :IVulkanObject;
import :VulkanSwapchain;
import :ShaderManager;
import :Mesh;
import :VertexBuffer;
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
        /**
         *@param vertexPath - path to the vertex shader file
         *@param fragmentPath - path to the fragment shader file
        */
        Pipeline(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
            const vk::Extent2D& extent, const QueueFamilyIndices& queueFamilyIndices, const std::string_view& vertexPath, const std::string_view& fragmentPath, const Mesh_t& mesh = {},
            const Types::uint& inPosLocation = 0, const Types::uint& inColorLocation = 1);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void Render(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices, const Scene& scene);

        /**
         * @brief Must be called before Render method
         */
        void SetMesh(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const Mesh_t& mesh);
    private:
        static vk::PipelineLayout CreateLayout(const vk::Device& device);
        static vk::RenderPass CreateRenderPass(const vk::Device& device, const vk::Format& swapchainImageFormat);
        static vk::Queue GetGraphicsQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept;
        static vk::Queue Getpresent(const vk::Device& device, const QueueFamilyIndices& indices) noexcept;

        void Create(const vk::Device& device, const vk::Extent2D& extent, const std::string_view& vertexPath, const std::string_view& fragmentPath);

        void Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices);

        void RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const Types::uint& imageIndex, const Scene& scene) const;

        ShaderManager m_ShaderManager;
        Swapchain m_Swapchain;

        VertexBuffer m_VBuffer;

        vk::RenderPass m_RenderPass;
        vk::PipelineLayout m_Layout;
        vk::Pipeline m_GraphicsPipeline;

        struct
        {
            vk::Queue graphics;
            vk::Queue present;
        } m_Queues;
    };
}