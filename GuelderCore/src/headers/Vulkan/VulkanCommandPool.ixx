module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanCommandPool;

import :IVulkanObject;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct VulkanSwapchainFrame;
    struct VulkanQueueFamilyIndices;

    class VulkanCommandPool : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanCommandPool);
        
        VulkanCommandPool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices);
        /**
         * \brief Allocates command buffers for frames
        */
        VulkanCommandPool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, std::vector<VulkanSwapchainFrame>& frames);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        vk::CommandBuffer CreateCommandBuffer(const vk::Device& device) const;
        static void MakeCommandBuffersForFrames(const vk::Device& device, const vk::CommandPool& pool, std::vector<VulkanSwapchainFrame>& frames);
        void MakeCommandBuffersForFrames(const vk::Device& device, std::vector<VulkanSwapchainFrame>& frames) const;

        void FreeCommandBuffer(const vk::Device& device, const vk::CommandBuffer& commandBuffer) const noexcept;

    private:
        static vk::CommandPool MakePool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices);
        static vk::CommandBuffer MakeBuffer(const vk::Device& device, const vk::CommandPool& pool);

        vk::CommandPool m_CommandPool;

        //vk::CommandBuffer m_CommandPool;
    };
}