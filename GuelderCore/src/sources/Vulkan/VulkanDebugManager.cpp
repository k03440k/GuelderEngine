module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :VulkanDebugManager;

import GuelderEngine.Debug;

import <vector>;

namespace GuelderEngine::Vulkan
{
    VulkanDebugLayersManager::VulkanDebugLayersManager(const std::vector<ValidationLayer>& layers)
        : m_Layers(layers)
    {
        GE_CORE_CLASS_ASSERT(AreValidationLayersSupported(layers), "validation layers are not supported");
    }
    bool VulkanDebugLayersManager::AreValidationLayersSupported(const std::vector<ValidationLayer>& layers)
    {
        GE_CORE_CLASS_ASSERT(layers.size() > 0, "layers size is zero");

        const std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("Device can support following layers:");
        for (const auto& layer : supportedLayers)
        {
            Debug::LogInfo('\t', layer.layerName);
        }
#endif // GE_DEBUG_VULKAN

        bool found = false;
        for (const auto& extension : layers)
        {
            found = false;
            for (const auto& supportedExtension : supportedLayers)
            {
                if (strcmp(extension, supportedExtension.layerName) == 0)
                {
                    found = true;
#ifdef GE_DEBUG_VULKAN
                    Debug::LogInfo("Layer \"", extension, "\" is supported");
#endif //GE_DEBUG_VULKAN
                }
            }
            if (!found)
            {
#ifdef GE_DEBUG_VULKAN
                Debug::LogInfo("Layer \"", extension, "\" is not supported");
#endif //GE_DEBUG_VULKAN
                return false;
            }
        }

        return true;
    }
    VulkanDebugManager::VulkanDebugManager(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi)
        : m_DebugMessenger(CreateDebugMessenger(instance, dldi))
    {
    }
    void VulkanDebugManager::Reset()
    {
        m_DebugMessenger = nullptr;
    }
    void VulkanDebugManager::Cleanup(const vk::Instance& instance, const vk::DispatchLoaderDynamic& DLDI) const
    {
        instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, DLDI);
    }
    void VulkanDebugManager::LogDeviceProperties(const vk::PhysicalDevice& device)
    {
        const vk::PhysicalDeviceProperties properties = device.getProperties();

        Debug::LogInfo("Device name: ", properties.deviceName, ". Device driver version: ", properties.driverVersion);
        const vk::PhysicalDeviceMemoryProperties memoryProperties = device.getMemoryProperties();

        //device type
        {
            std::string msg = "Device type: ";
            switch(properties.deviceType)
            {
                using enum vk::PhysicalDeviceType;
            case (eCpu):
                msg += "CPU";
                break;
            case (eDiscreteGpu):
                msg += "Discrete GPU";
                break;
            case (eIntegratedGpu):
                msg += "Integrated GPU";
                break;
            case (eVirtualGpu):
                msg += "Virtual GPU";
                break;
            default:
                msg += "other";
            }
            Debug::LogInfo(msg);
        }

        const uint32_t heapCount = memoryProperties.memoryHeapCount;

        // Iterate over memory heaps to find the heap with the desired memory type
        for (uint32_t heapIndex = 0; heapIndex < heapCount; ++heapIndex) {
            // Check if the memory heap is for device local memory (typically used for graphics memory)
            if (const auto& heap = memoryProperties.memoryHeaps[heapIndex]; heap.flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
                // Convert the size to gigabytes
                const double memorySizeGB = static_cast<double>(heap.size) / (1024 * 1024 * 1024);
                Debug::LogInfo("Memory size in ", heapIndex+1, " heap : ", memorySizeGB, " GB");
            }
        }
    }
    vk::DebugUtilsMessengerEXT VulkanDebugManager::CreateDebugMessenger(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi)
    {
        constexpr vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
            vk::DebugUtilsMessengerCreateFlagsEXT(),
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
            | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
            | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
            DebugCallback,
            nullptr
        );

        return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
    }
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugManager::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                                     const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                                                                     void* userData)
    {
        Debug::LogVulkanError(callbackData->pMessage);

        return VK_FALSE;
    }
#pragma region operators_and_ctors
    VulkanDebugManager::VulkanDebugManager(const VulkanDebugManager& other)
    {
        m_DebugMessenger = other.m_DebugMessenger;
    }
    VulkanDebugManager::VulkanDebugManager(VulkanDebugManager&& other)
    {
        m_DebugMessenger = other.m_DebugMessenger;

        other.m_DebugMessenger = nullptr;
    }
    VulkanDebugManager& VulkanDebugManager::operator=(VulkanDebugManager&& other)
    {
        m_DebugMessenger = other.m_DebugMessenger;

        other.m_DebugMessenger = nullptr;

        return *this;
    }
    VulkanDebugManager& VulkanDebugManager::operator=(const VulkanDebugManager& other)
    {
        if(this == &other)
            return *this;

        m_DebugMessenger = other.m_DebugMessenger;
        return *this;
    }
#pragma endregion
}
