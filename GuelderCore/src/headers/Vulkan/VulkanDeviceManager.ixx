module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanDeviceManager;

import :IVulkanBase;
import :VulkanDebugManager;
import :VulkanSwapchain;
import :VulkanPipeline;
import GuelderEngine.Core.Types;

import <optional>;

export namespace GuelderEngine::Vulkan
{
    /*
     *You must manually call .Cleanup() func instead of dtor
     */
    class VulkanDeviceManager final : public IVulkanBase, INHERIT_GClass(VulkanDeviceManager)
    {
    public:
        VulkanDeviceManager() = default;
        VulkanDeviceManager(const vk::Instance& instance, GLFWwindow* glfwWindow, const std::string_view& vertPath, const std::string_view& fragPath);
        ~VulkanDeviceManager() = default;

        //DELETE_COPY(VulkanDeviceManager);

        VulkanDeviceManager(const VulkanDeviceManager& other);
        VulkanDeviceManager(VulkanDeviceManager&& other) noexcept;
        VulkanDeviceManager& operator=(const VulkanDeviceManager& other);
        VulkanDeviceManager& operator=(VulkanDeviceManager&& other) noexcept;

        void Reset() noexcept override;
        void Cleanup(const vk::Instance& instance) const noexcept;
    private:
        static bool CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice,
            const std::vector<const char*>& requestedExtensions);
        static bool IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice);
        static vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance& instance);
        static vk::Device CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices);
        static vk::Queue GetGraphicsQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept;
        static vk::Queue GetPresentQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept;

        friend class VulkanManager;

        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;
        QueueFamilyIndices m_QueueIndices;

        vk::SurfaceKHR m_Surface;
        struct
        {
            vk::Queue graphicsQueue;
            vk::Queue presentQueue;
        } m_Queues;

        VulkanSwapchain m_Swapchain;
        VulkanPipeline m_Pipeline;
    };
}