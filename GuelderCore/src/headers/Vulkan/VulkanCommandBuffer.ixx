module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanCommandBuffer;

import :IVulkanObject;
import :VulkanQueueFamilyIndices;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct VulkanSwapchainFrame;

    class VulkanCommandBuffer : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanCommandBuffer);

        VulkanCommandBuffer(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, const vk::SurfaceKHR& surface, std::vector<VulkanSwapchainFrame>& frames);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;
    private:
        static vk::CommandPool MakePool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, const vk::SurfaceKHR& surface);
        static vk::CommandBuffer MakeBuffer(const vk::Device& device, const vk::CommandPool& pool, std::vector<VulkanSwapchainFrame>& frames);

        vk::CommandPool m_CommandPool;
        vk::CommandBuffer m_CommandBuffer;
    };
}