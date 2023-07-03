module;
#include <../includes/GuelderEngine/Utils/Debug.hpp>
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :VulkanDeviceManager;

import :VulkanDebugManager;
import :VulkanSurfaceManager;
import GuelderEngine.Core.Types;

import <optional>;
import <set>;

namespace GuelderEngine::Vulkan
{
    QueueFamilyIndices::QueueFamilyIndices()
        : graphicsFamily(0), presentFamily(0) {}

    VulkanDeviceManager::VulkanDeviceManager(const vk::Instance& instance, const VulkanSurfaceManager& surface)
    {
        m_PhysicalDevice = ChoosePhysicalDevice(instance);
        m_QueueIndices = FindQueueFamily(m_PhysicalDevice, surface);
        m_Device = CreateDevice(m_PhysicalDevice, m_QueueIndices, surface);
        m_Queues.graphicsQueue = GetGraphicsQueue(m_PhysicalDevice, m_Device, m_QueueIndices);
        m_Queues.presentQueue = GetPresentQueue(m_PhysicalDevice, m_Device, m_QueueIndices);
    }
    void VulkanDeviceManager::Cleanup() const
    {
        m_Device.destroy();
    }
    VulkanDeviceManager& VulkanDeviceManager::operator=(const VulkanDeviceManager& other)
    {
        m_PhysicalDevice = other.m_PhysicalDevice;
        m_Device = other.m_Device;
        return *this;
    }
    QueueFamilyIndices VulkanDeviceManager::FindQueueFamily(const vk::PhysicalDevice& device, const VulkanSurfaceManager& surface)
    {
        QueueFamilyIndices indices;

        const std::vector queueFamilies = device.getQueueFamilyProperties();

#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("Device can support ", queueFamilies.size(), " Queue Families");
#endif

        for (size_t i = 0; i < queueFamilies.size(); i++)
        {
            if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics && device.getSurfaceSupportKHR(i, surface.surface))
            {
                indices.graphicsFamily = i;
                indices.presentFamily = i;

#ifdef GE_DEBUG_VULKAN
                Debug::LogInfo("Queue Family at index ", i, " is suitable for graphics and presenting");
#endif
            }
            if (indices.IsComplete())
                break;
        }

        GE_CORE_CLASS_ASSERT(indices.IsComplete(), "Cannot complete Queue Family(device doesn't support requirements)");

        return indices;
    }
    bool VulkanDeviceManager::CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& requestedExtensions)
    {
        std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());
#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("Device can support following extensions:");
#endif

        for (auto&& extension : physicalDevice.enumerateDeviceExtensionProperties())
        {
#ifdef GE_DEBUG_VULKAN
            Debug::LogInfo('\t', extension.extensionName);
#endif
            //remove this from the list of required extensions (set checks for equality automatically)
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
    bool VulkanDeviceManager::IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice)
    {
        if (const std::vector requestedExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
            !CheckDeviceExtensionsSupport(physicalDevice, requestedExtensions))
        {
            //Debug::LogError("Device cannot support requested extensions");
            return false;
        }

        return true;
    }
    vk::PhysicalDevice VulkanDeviceManager::ChoosePhysicalDevice(const vk::Instance& instance)
    {
        const std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

        GE_CORE_CLASS_ASSERT(physicalDevices.size() > 0, "there are no physical devices");

#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("There are ", physicalDevices.size(), " detected physical devices:");
        for (const auto& device : physicalDevices)
        {
            VulkanDebugManager::LogDeviceProperties(device);
        }
#endif // GE_DEBUG_VULKAN
        //device choosing

        double theBiggestDeviceMemorySize{};
        Types::uint idxToDeviceOftheBiggestMemory{};
        //takes the most powerful device
        for (Types::uint i = 0; i < physicalDevices.size(); i++)
        {
            const vk::PhysicalDeviceMemoryProperties& memoryProperties = physicalDevices[i].getMemoryProperties();

            const uint32_t heapCount = memoryProperties.memoryHeapCount;

            double memoryOfWholeDevice{};

            // Iterate over memory heaps to find the heap with the desired memory type
            for (uint32_t heapIndex = 0; heapIndex < heapCount; ++heapIndex) {
                // Check if the memory heap is for device local memory (typically used for graphics memory)
                if (const auto& heap = memoryProperties.memoryHeaps[heapIndex]; heap.flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
                    // Convert the size to gigabytes
                    memoryOfWholeDevice += static_cast<double>(heap.size) / (1024 * 1024 * 1024);
                }
            }
            if (theBiggestDeviceMemorySize < memoryOfWholeDevice && IsDeviceSuitable(physicalDevices[i]))
            {
                theBiggestDeviceMemorySize = memoryOfWholeDevice;
                idxToDeviceOftheBiggestMemory = i;
            }
        }

        GE_CORE_CLASS_ASSERT(theBiggestDeviceMemorySize > 0, "cannot choose device");

        return physicalDevices[idxToDeviceOftheBiggestMemory];//TODO: Queue Families at the video time of 1:04:11
    }
    vk::Device VulkanDeviceManager::CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const VulkanSurfaceManager& surface)
    {
        constexpr float queuePriority = 1.0f;

        std::vector uniqueIndices{indices.graphicsFamily.value()};
        if (indices.graphicsFamily.value() != indices.presentFamily.value())
            uniqueIndices.push_back(indices.presentFamily.value());//9:11

        std::vector<vk::DeviceQueueCreateInfo> queueDeviceInfo;
        for (auto&& queueFamilyIndex : uniqueIndices)
        {
            queueDeviceInfo.push_back(
                vk::DeviceQueueCreateInfo(
                vk::DeviceQueueCreateFlags(),
                queueFamilyIndex,
                1,
                &queuePriority));
        }

        const auto deviceFeatures = physicalDevice.getFeatures();//

        std::vector<const char*> enabledLayers;
#ifdef GE_DEBUG_VULKAN
        enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        const auto deviceInfo = vk::DeviceCreateInfo(
            vk::DeviceCreateFlags(),
            queueDeviceInfo.size(),
            queueDeviceInfo.data(),
            enabledLayers.size(),
            enabledLayers.data(),
            0,
            nullptr,
            &deviceFeatures
        );

        return physicalDevice.createDevice(deviceInfo);
    }
    vk::Queue VulkanDeviceManager::GetGraphicsQueue(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const QueueFamilyIndices& indices)
    {
        return device.getQueue(indices.graphicsFamily.value(), 0);
    }
    vk::Queue VulkanDeviceManager::GetPresentQueue(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const QueueFamilyIndices& indices)
    {
        return device.getQueue(indices.presentFamily.value(), 0);
    }
}