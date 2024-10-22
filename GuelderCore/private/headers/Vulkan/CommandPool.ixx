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
        DECLARE_DEFAULT_CTOR_AND_DTOR(CommandPool);
        DECLARE_DEFAULT_COPYING(CommandPool);
        DECLARE_MOVING(CommandPool);

        CommandPool(const vk::Device& device, const QueueFamilyIndices& queueFamilyIndices, const vk::QueueFlagBits& queueType = vk::QueueFlagBits::eGraphics);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;
        
        static vk::CommandBuffer CreateCommandBuffer(const vk::Device& device, const vk::CommandPool& commandPool);
        static vk::CommandBuffer BeginSingleTimeSubmitCommandBuffer(const vk::Device& device, const vk::CommandPool& commandPool);

        static void FreeCommandBuffer(const vk::Device& device, const vk::CommandPool& commandPool, const vk::CommandBuffer& commandBuffer) noexcept;

        vk::CommandBuffer CreateCommandBuffer(const vk::Device& device) const;
        vk::CommandBuffer BeginSingleTimeSubmitCommandBuffer(const vk::Device& device) const;
        
        void FreeCommandBuffer(const vk::Device& device, const vk::CommandBuffer& commandBuffer) const noexcept;

        const vk::CommandPool& GetCommandPool() const noexcept;

    private:
        vk::CommandPool m_CommandPool;
    };
}