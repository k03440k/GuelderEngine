module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :DebugManager;

import :IVulkanObject;
import GuelderEngine.Debug;

import <vector>;

namespace GuelderEngine::Vulkan
{
    //DEFINE_LOG_CATEGORY(VulkanCore);

#pragma region operators_and_ctors
    DebugManager::DebugManager(const vk::Instance& instance)
    {
        m_DLDI = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
        m_DebugMessenger = CreateDebugMessenger(instance, m_DLDI);
    }
    DebugManager::DebugManager(const DebugManager& other)
    {
        m_DebugMessenger = other.m_DebugMessenger;
        m_DLDI = other.m_DLDI;
    }
    DebugManager::DebugManager(DebugManager&& other) noexcept
    {
        m_DebugMessenger = other.m_DebugMessenger;
        m_DLDI = other.m_DLDI;

        //other.Reset();//idk why error occurs
    }
    DebugManager& DebugManager::operator=(const DebugManager& other)
    {
        if(this == &other)
            return *this;

        m_DebugMessenger = other.m_DebugMessenger;
        m_DLDI = other.m_DLDI;

        return *this;
    }
    DebugManager& DebugManager::operator=(DebugManager&& other) noexcept
    {
        m_DebugMessenger = other.m_DebugMessenger;
        m_DLDI = other.m_DLDI;

        //other.Reset();//idk why error occurs

        return *this;
    }
#pragma endregion

    DebugLayersManager::DebugLayersManager(const std::vector<ValidationLayer>& layers)
        : m_Layers(layers)
    {
        GE_ASSERT(AreValidationLayersSupported(layers), "validation layers are not supported");
    }
    bool DebugLayersManager::AreValidationLayersSupported(const std::vector<ValidationLayer>& layers)
    {
        GE_ASSERT(layers.size() > 0, "layers size is zero");

        const std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

#ifdef GE_DEBUG_VULKAN
        /*GE_LOG(VulkanCore, Info, "Device can support following layers:");
        for (const auto& layer : supportedLayers)
        {
            GE_LOG(VulkanCore, Info, '\t', layer.layerName);
        }*/
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

                    //GE_LOG(VulkanCore, Info, "Layer \"", extension, "\" is supported");
                }
            }
            if (!found)
            {
                //GE_LOG(VulkanCore, Info, "Layer \"", extension, "\" is not supported");

                return false;
            }
        }

        return true;
    }

    void DebugManager::Reset() noexcept
    {
        m_DebugMessenger = nullptr;
        m_DLDI = nullptr;
    }
    void DebugManager::Cleanup(const vk::Instance& instance) const noexcept
    {
        instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DLDI);
    }
    void DebugManager::LogDeviceProperties(const vk::PhysicalDevice& device)
    {
        const vk::PhysicalDeviceProperties properties = device.getProperties();

        GE_LOG(VulkanCore, Info, "Device name: ", properties.deviceName, ". Device driver version: ", properties.driverVersion);
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
            GE_LOG(VulkanCore, Info, msg);
        }

        const uint32_t heapCount = memoryProperties.memoryHeapCount;

        // Iterate over memory heaps to find the heap with the desired memory type
        for (uint32_t heapIndex = 0; heapIndex < heapCount; ++heapIndex) {
            // Check if the memory heap is for device local memory (typically used for graphics memory)
            if (const auto& heap = memoryProperties.memoryHeaps[heapIndex]; heap.flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
                // Convert the size to gigabytes
                const double memorySizeGB = static_cast<double>(heap.size) / (1024 * 1024 * 1024);
                GE_LOG(VulkanCore, Info, "Memory size in ", heapIndex+1, " heap : ", memorySizeGB, " GB");
            }
        }

        GE_LOG(VulkanCore, Info, "Max push constants memory(in bytes) is ", properties.limits.maxPushConstantsSize);
    }
    void DebugManager::LogPresentMode(const vk::PresentModeKHR& mode)
    {
        using enum vk::PresentModeKHR;
        if(mode == eMailbox)
            GE_LOG(VulkanCore, Info, "\tMailBox");
        if(mode == eImmediate)
            GE_LOG(VulkanCore, Info, "\tImmediate");
        if(mode == eFifo)
            GE_LOG(VulkanCore, Info, "\tFifo");
        if(mode == eFifoRelaxed)
            GE_LOG(VulkanCore, Info, "\tFifoRelaxed");
        if(mode == eSharedContinuousRefresh)
            GE_LOG(VulkanCore, Info, "\tSharedContinuousRefresh");
        if(mode == eSharedDemandRefresh)
            GE_LOG(VulkanCore, Info, "\tSharedDemandRefresh");
    }
    vk::DebugUtilsMessengerEXT DebugManager::CreateDebugMessenger(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi)
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
    VKAPI_ATTR VkBool32 VKAPI_CALL DebugManager::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                                     const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                                                                     void* userData)
    {
        GE_LOG(VulkanCore, VulkanError, callbackData->pMessage);

        return VK_FALSE;
    }
}