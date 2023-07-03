module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanDeviceManager;

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
    class VulkanDeviceManager final : INHERIT_GClass(VulkanDeviceManager)
    {
    public:
        VulkanDeviceManager() = default;
        VulkanDeviceManager(const vk::Instance& instance, const class VulkanSurfaceManager& surface);
        ~VulkanDeviceManager() = default;

        void Cleanup() const;

        VulkanDeviceManager& operator=(const VulkanDeviceManager & other);
    private:
        static bool CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice,
            const std::vector<const char*>& requestedExtensions);
        static bool IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice);
        static vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance& instance);
        static vk::Device CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const VulkanSurfaceManager& surface);
        static vk::Queue GetGraphicsQueue(const vk::PhysicalDevice& physicalDevice, const vk::Device & device, const QueueFamilyIndices & indices);
        static vk::Queue GetPresentQueue(const vk::PhysicalDevice& physicalDevice, const vk::Device & device, const QueueFamilyIndices & indices);
        static QueueFamilyIndices FindQueueFamily(const vk::PhysicalDevice & device, const VulkanSurfaceManager& surface);

        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;
        QueueFamilyIndices m_QueueIndices;

        struct
        {
            vk::Queue graphicsQueue;
            vk::Queue presentQueue;
        } m_Queues;
    };
}