module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanDeviceManager;

import :IVulkanBase;
import GuelderEngine.Core.Types;

import <optional>;

export namespace GuelderEngine::Vulkan
{
    struct QueueFamilyIndices
    {
        QueueFamilyIndices();
        ~QueueFamilyIndices() = default;

        //location of graphics Queue Family
        std::optional<Types::uint> graphicsFamily;
        std::optional<Types::uint> presentFamily;

        bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };
    /*
     *You must manually call .Cleanup() func instead of dtor
     */
    class VulkanDeviceManager final : public IVulkanBase, INHERIT_GClass(VulkanDeviceManager)
    {
    public:
        VulkanDeviceManager() = default;
        VulkanDeviceManager(const vk::Instance& instance, GLFWwindow* glfwWindow);
        ~VulkanDeviceManager() = default;

        //DELETE_COPY(VulkanDeviceManager);

        VulkanDeviceManager(const VulkanDeviceManager& other);
        VulkanDeviceManager(VulkanDeviceManager&& other);
        VulkanDeviceManager& operator=(const VulkanDeviceManager& other);
        VulkanDeviceManager& operator=(VulkanDeviceManager&& other);

        void Reset() override;
        void Cleanup(const vk::Instance& instance) const;
    private:
        static bool CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice,
            const std::vector<const char*>& requestedExtensions);
        static bool IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice);
        static vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance& instance);
        static vk::Device CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices);
        static vk::Queue GetGraphicsQueue(const vk::Device & device, const QueueFamilyIndices & indices);
        static vk::Queue GetPresentQueue(const vk::Device & device, const QueueFamilyIndices & indices);
        static QueueFamilyIndices FindQueueFamily(const vk::PhysicalDevice & device, const vk::SurfaceKHR & surface);

        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;
        QueueFamilyIndices m_QueueIndices;

        vk::SurfaceKHR m_Surface;
        struct
        {
            vk::Queue graphicsQueue;
            vk::Queue presentQueue;
        } m_Queues;
    };
}