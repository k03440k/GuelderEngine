module;
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include "../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:Renderer;

import :IVulkanObject;
import :Swapchain;
import :DeviceManager;
import :Surface;

import <functional>;

export namespace GuelderEngine::Vulkan
{
    //class DeviceManager;
    struct QueueFamilyIndices;

    class Renderer : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(Renderer);
        DECLARE_MOVING(Renderer);
        
        Renderer(GLFWwindow* window, const vk::Instance& instance, const DeviceManager& deviceManager, uint width, uint height);
        Renderer(
            GLFWwindow* window,
            const vk::Instance& instance,
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::Extent2D& extent,
            const vk::CommandPool& commandPool,
            const QueueFamilyIndices& queueFamilyIndices
        );

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device, const vk::Instance& instance, const vk::CommandPool& commandPool) const noexcept;
        FORCE_INLINE void Cleanup(const DeviceManager& deviceManager, const vk::Instance& instance) const noexcept { Cleanup(deviceManager.GetDevice(), instance, deviceManager.GetCommandPool().GetCommandPool()); }

        /// <returns>
        /// bool whether to start frame
        /// </returns>
        bool BeginFrame(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::CommandPool& commandPool,
            const std::vector<vk::CommandBuffer>& commandBuffers,
            const vk::Extent2D& extent,
            const QueueFamilyIndices& queueFamilyIndices
        );
        FORCE_INLINE bool BeginFrame(const DeviceManager& deviceManager, const std::vector<vk::CommandBuffer>& commandBuffers, const vk::Extent2D& extent)
        {
            return BeginFrame(
                deviceManager.GetDevice(),
                deviceManager.GetPhysicalDevice(),
                deviceManager.GetCommandPool().GetCommandPool(),
                commandBuffers,
                extent,
                deviceManager.GetQueueIndices()
            );
        }

        /// <returns>
        /// whether the recreation was done
        /// </returns>
        bool EndFrame(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const vk::CommandPool& commandPool,
            const std::vector<vk::CommandBuffer>& commandBuffers,
            const vk::Queue& graphicsQueue,
            const vk::Queue& presentQueue,
            const vk::Extent2D& extent,
            bool& wasWindowResized,
            const QueueFamilyIndices& queueFamilyIndices
        );
        FORCE_INLINE bool EndFrame(const DeviceManager& deviceManager, const std::vector<vk::CommandBuffer>& commandBuffers, const vk::Extent2D& extent, bool& wasWindowResized)
        {
            return EndFrame(
                deviceManager.GetDevice(),
                deviceManager.GetPhysicalDevice(),
                deviceManager.GetCommandPool().GetCommandPool(),
                commandBuffers,
                deviceManager.GetQueues().graphics,
                deviceManager.GetQueues().present,
                extent,
                wasWindowResized,
                deviceManager.GetQueueIndices()
            );
        }

        void BeginSwapchainRenderPass(const vk::CommandBuffer& commandBuffer) const;
        void EndSwapchainRenderPass(const vk::CommandBuffer& commandBuffer) const;

        void IncrementCurrentFrame();

        const Swapchain& GetSwapchain() const;
        const Surface& GetSurface() const;
        bool IsFrameInProgress() const noexcept;
    private:
        Swapchain m_Swapchain;
        Surface m_Surface;

        uint m_CurrentImageIndex;
        bool m_IsFrameStarted;
    };
}