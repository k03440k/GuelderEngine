module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:CommandPool;

import :IVulkanObject;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct SwapchainFrame;
    struct QueueFamilyIndices;

    class CommandPool : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(CommandPool);
        
        CommandPool(const vk::Device& device, const QueueFamilyIndices& queueFamilyIndices);
        /**
         * @brief Allocates command buffers for frames
        */
        CommandPool(const vk::Device& device, const QueueFamilyIndices& queueFamilyIndices, std::vector<SwapchainFrame>& frames);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        vk::CommandBuffer CreateCommandBuffer(const vk::Device& device) const;
        static void MakeCommandBuffersForFrames(const vk::Device& device, const vk::CommandPool& pool, std::vector<SwapchainFrame>& frames);
        void MakeCommandBuffersForFrames(const vk::Device& device, std::vector<SwapchainFrame>& frames) const;

        void FreeCommandBuffer(const vk::Device& device, const vk::CommandBuffer& commandBuffer) const noexcept;

    private:
        static vk::CommandPool MakePool(const vk::Device& device, const QueueFamilyIndices& queueFamilyIndices);
        static vk::CommandBuffer MakeBuffer(const vk::Device& device, const vk::CommandPool& pool);

        vk::CommandPool m_CommandPool;
    };
}