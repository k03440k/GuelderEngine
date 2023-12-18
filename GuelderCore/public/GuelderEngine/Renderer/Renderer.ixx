//TODO: move this file to vulkan folder
module;
#include <vulkan/vulkan.hpp>
#include "../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:Renderer;

import :IVulkanObject;
import :Swapchain;
import :Mesh;
import :DeviceManager;
import :VertexBuffer;
import :IndexBuffer;

export namespace GuelderEngine::Vulkan
{
    //class DeviceManager;
    struct QueueFamilyIndices;

    class Renderer : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(Renderer);
        DECLARE_MOVING(Renderer);

        //for the swapchain initialization
        Renderer(const DeviceManager& deviceManager, uint width, uint height);
        Renderer(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::SurfaceKHR& surface,
            const vk::Extent2D& extent,
            const vk::CommandPool& commandPool,
            const QueueFamilyIndices& queueFamilyIndices
        );

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device, const vk::CommandPool& commandPool) const noexcept;
        FORCE_INLINE void Cleanup(const DeviceManager& deviceManager) const noexcept { Cleanup(deviceManager.GetDevice(), deviceManager.GetCommandPool().GetCommandPool()); }

        //could be reference, because the frame is stored inside the swapchain which doesn't remove them(I guess)
        vk::CommandBuffer BeginFrame(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::SurfaceKHR& surface,
            const vk::CommandPool& commandPool,
            const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices
        );
        FORCE_INLINE vk::CommandBuffer BeginFrame(const DeviceManager& deviceManager, const vk::Extent2D& extent)
        {
            return BeginFrame(
                deviceManager.GetDevice(), 
                deviceManager.GetPhysicalDevice(),
                deviceManager.GetSurface(), 
                deviceManager.GetCommandPool().GetCommandPool(), 
                extent, deviceManager.GetQueueIndices()
            );
        }
        void EndFrame(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::SurfaceKHR& surface,
            const vk::CommandPool& commandPool,
            const vk::Queue& graphicsQueue,
            const vk::Queue& presentQueue,
            const vk::Extent2D& extent,
            bool& wasWindowResized,
            const QueueFamilyIndices& queueFamilyIndices
        );
        FORCE_INLINE void EndFrame(const DeviceManager& deviceManager, const vk::Extent2D& extent, bool& wasWindowResized)
        {
            EndFrame(
                deviceManager.GetDevice(),
                deviceManager.GetPhysicalDevice(),
                deviceManager.GetSurface(),
                deviceManager.GetCommandPool().GetCommandPool(),
                deviceManager.GetQueues().graphics,
                deviceManager.GetQueues().present,
                extent,
                wasWindowResized,
                deviceManager.GetQueueIndices()
            );
        }
        void BeginSwapchainRenderPass(const vk::CommandBuffer& commandBuffer) const;
        void EndSwapchainRenderPass(const vk::CommandBuffer& commandBuffer) const;

        const Swapchain& GetSwapchain() const;
        bool IsFrameInProgress() const noexcept;
    private:
        Swapchain m_Swapchain;

        uint m_CurrentImageIndex;
        bool m_IsFrameStarted;
    };
}