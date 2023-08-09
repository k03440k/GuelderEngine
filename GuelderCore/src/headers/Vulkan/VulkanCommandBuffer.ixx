module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanCommandBuffer;

import :IVulkanObject;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct VulkanSwapchainFrame;
    struct VulkanQueueFamilyIndices;

    class VulkanCommandBuffer : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanCommandBuffer);
        
        VulkanCommandBuffer(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices);
        /**
         * \brief Allocates command buffers for frames
        */
        VulkanCommandBuffer(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, std::vector<VulkanSwapchainFrame>& frames);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;
        
        static void MakeFrameBuffers(const vk::Device& device, const vk::CommandPool& pool, std::vector<VulkanSwapchainFrame>& frames);
        void MakeFrameBuffers(const vk::Device& device, std::vector<VulkanSwapchainFrame>& frames) const;

    private:
        static vk::CommandPool MakePool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices);
        static vk::CommandBuffer MakeBuffer(const vk::Device& device, const vk::CommandPool& pool);

        vk::CommandPool m_CommandPool;

        //vk::CommandBuffer m_CommandBuffer;
    };
}