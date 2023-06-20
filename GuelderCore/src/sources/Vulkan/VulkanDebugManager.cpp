module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :VulkanDebugManager;

import GuelderEngine.Debug;

import <vector>;

namespace GuelderEngine
{
    namespace Vulkan
    {
        VulkanDebugLayersManager::VulkanDebugLayersManager(const std::vector<ValidationLayer>& layers)
            : m_Layers(layers)
        {
            GE_CORE_ASSERT(AreValidationLayersSupported(layers), "validation layers are not supported");
        }
        bool VulkanDebugLayersManager::AreValidationLayersSupported(const std::vector<ValidationLayer>& layers)
        {
            GE_CORE_ASSERT(layers.size() > 0, "layers size is zero");

            const std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

#ifdef DEBUG_VULKAN
            Debug::LogInfo("Device can support following layers:");
            for (const auto& layer : supportedLayers)
            {
                Debug::LogInfo('\t', layer.layerName);
            }
#endif // DEBUG_VULKAN

            bool found = false;
            for (const auto& extension : layers)
            {
                found = false;
                for (const auto& supportedExtension : supportedLayers)
                {
                    if (strcmp(extension, supportedExtension.layerName) == 0)
                    {
                        found = true;
#ifdef DEBUG_VULKAN
                        Debug::LogInfo("Layer \"", extension, "\" is supported");
#endif //DEBUG_VULKAN
                    }
                }
                if (!found)
                {
#ifdef DEBUG_VULKAN
                    Debug::LogInfo("Layer \"", extension, "\" is not supported");
#endif //DEBUG_VULKAN
                    return false;
                }
            }

            return true;
        }
        VulkanDebugManager::VulkanDebugManager(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi)
            : m_DebugMessenger(CreateDebugMessenger(instance, dldi))
        {
        }
        VulkanDebugManager& VulkanDebugManager::operator=(const VulkanDebugManager& other)
        {
            m_DebugMessenger = other.m_DebugMessenger;
            return *this;
        }
        void VulkanDebugManager::LogDeviceProperties(const vk::PhysicalDevice& device)
        {
            const vk::PhysicalDeviceProperties properties = device.getProperties();

            Debug::LogInfo("Device name: ", properties.deviceName, ". Device driver version: ", properties.driverVersion);
        }
        vk::DebugUtilsMessengerEXT VulkanDebugManager::CreateDebugMessenger(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi)
        {
            vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
                vk::DebugUtilsMessengerCreateFlagsEXT(),
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
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
    }
}