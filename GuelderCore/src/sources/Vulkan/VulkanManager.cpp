module;
#include "../../../includes/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
module GuelderEngine.Vulkan;
import :VulkanManager;

import :VulkanDebugManager;
import GuelderEngine.Debug;
import GuelderEngine.Core.Types;

import <vector>;
import <string_view>;

namespace GuelderEngine
{
    namespace Vulkan
    {
#pragma region VulkanManager
        VulkanManager::VulkanManager(const std::string_view& name)
        {
            Init(name);
        }
        void VulkanManager::Init(const std::string_view& name)
        {
            VkInit(name.data());
        }
        void VulkanManager::VkInit(const char* const name)
        {
            m_Instance = CreateVkInstance(name);
            m_DLDI = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);

#ifdef DEBUG_VULKAN
            m_DebugManager = Vulkan::VulkanDebugManager(m_Instance, m_DLDI);
#endif // DEBUG_VULKAN
            m_DeviceManager = VulkanDeviceManager(m_Instance);
        }
        vk::Instance VulkanManager::CreateVkInstance(const char* const name)
        {
            Types::uint version{};
            vkEnumerateInstanceVersion(&version);
#ifdef DEBUG_VULKAN
            Debug::LogInfo("System can support Vulkan variant: ", VK_API_VERSION_VARIANT(version), ", ",
                VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version));
#endif // DEBUG_VULKAN

            vk::ApplicationInfo appInfo(name, 0, "Guelder Engine", version);

            Types::uint glfwExtensionsCount{};
            const char** const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

#ifdef DEBUG_VULKAN
            extensions.push_back("VK_EXT_debug_utils");

            if (extensions.size() > 0)
            {
                Debug::LogInfo("All available extensions for Vulkan are:");
                for (const auto extension : extensions)
                {
                    Debug::LogInfo(extension);
                }
            }
#endif // DEBUG_VULKAN

            GE_CORE_ASSERT(AreExtensionsSupported(extensions), "extensions are not supported");

#ifdef DEBUG_VULKAN
            //m_DebugManager = VulkanDebugManager(m_Instance, m_DLDI, { "VK_LAYER_KHRONOS_validation" });

            //std::vector<const char*> layers;
            //layers.push_back("VK_LAYER_KHRONOS_validation");
            //GE_CORE_ASSERT(IsValidationLayersSupported(layers), "validation layers are not supported");

            Vulkan::VulkanDebugLayersManager layers({ "VK_LAYER_KHRONOS_validation" });

            vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
                &appInfo,
                static_cast<Types::uint>(layers.GetLayers().size()), layers.GetLayers().data(),//debug layers
                (Types::uint)extensions.size(), extensions.data());//extensions
#else
            vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
                &appInfo,
                0, nullptr),//debug layers
                (Utils::uint)extensions.size(), extensions.data());//extensions
#endif // DEBUG_VULKAN
                return vk::createInstance(createInfo);
        }
        /*VulkanDebugManager::VulkanDebugManager(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi,
            const std::vector<const char* const>& validationLayers) : m_ValidationLayers(validationLayers)
        {
            m_DebugMessenger = CreateDebugMessenger(instance, dldi);
            GE_CORE_ASSERT(AreValidationLayersSupported(validationLayers), "layers are not supported");
        }*/
        bool VulkanManager::AreExtensionsSupported(const std::vector<const char*>& extensions)
        {
            GE_CORE_ASSERT(extensions.size() > 0, "extensions size is zero");

            std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

#ifdef DEBUG_VULKAN
            Debug::LogInfo("Device can support following extensions:");
            for (const auto& supportedExtension : supportedExtensions)
            {
                Debug::LogInfo('\t', supportedExtension.extensionName);
            }
#endif //DEBUG_VULKAN

            bool found = false;
            for (const auto& extension : extensions)
            {
                found = false;
                for (const auto& supportedExtension : supportedExtensions)
                {
                    if (strcmp(extension, supportedExtension.extensionName) == 0)
                    {
                        found = true;
#ifdef DEBUG_VULKAN
                        Debug::LogInfo("Extension \"", extension, "\" is supported");
#endif //DEBUG_VULKAN
                    }
                }
                if (!found)
                {
#ifdef DEBUG_VULKAN
                    Debug::LogInfo("Extension \"", extension, "\" is not supported");
#endif //DEBUG_VULKAN
                    return false;
                }
            }

            return true;
        }
        //    bool VulkanManager::IsValidationLayersSupported(const std::vector<const char*>& layers)
        //    {
        //        GE_CORE_ASSERT(layers.size() > 0, "layers size is zero");
        //
        //        std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
        //
        //#ifdef DEBUG_VULKAN
        //        Logger::Log(LogLevel::Info, "Device can support following layers:");
        //        for (const auto& layer : supportedLayers)
        //        {
        //            Logger::Log(LogLevel::Info, '\t', layer.layerName);
        //        }
        //#endif // DEBUG_VULKAN
        //
        //        bool found = false;
        //        for (const auto& extension : layers)
        //        {
        //            found = false;
        //            for (const auto& supportedExtension : supportedLayers)
        //            {
        //                if (strcmp(extension, supportedExtension.layerName) == 0)
        //                {
        //                    found = true;
        //#ifdef DEBUG_VULKAN
        //                    Logger::Log(LogLevel::Info, "Layer \"", extension, "\" is supported");
        //#endif //DEBUG_VULKAN
        //                }
        //            }
        //            if (!found)
        //            {
        //#ifdef DEBUG_VULKAN
        //                Logger::Log(LogLevel::Info, "Layer \"", extension, "\" is not supported");
        //#endif //DEBUG_VULKAN
        //                return false;
        //            }
        //        }
        //
        //        return true;
        //    }
        void VulkanManager::Cleanup() const noexcept
        {
#ifdef DEBUG_VULKAN
            m_Instance.destroyDebugUtilsMessengerEXT(m_DebugManager.m_DebugMessenger, nullptr, m_DLDI);
#endif // DEBUG_VULKAN
            m_Instance.destroy();
        }
        VulkanManager::~VulkanManager()
        {
            Cleanup();
        }
#pragma endregion
        VulkanDeviceManager::VulkanDeviceManager(const vk::Instance& instance)
        {
            m_PhysicalDevice = ChoosePhysicalDevice(instance);
        }
        VulkanDeviceManager& VulkanDeviceManager::operator=(const VulkanDeviceManager& other)
        {
            m_PhysicalDevice = other.m_PhysicalDevice;
            return *this;
        }
        vk::PhysicalDevice VulkanDeviceManager::ChoosePhysicalDevice(const vk::Instance& instance)
        {
            const std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

#ifdef DEBUG_VULKAN
            Debug::LogInfo("There are ", physicalDevices.size(), " detected physical devices:");
            for (const auto& device : physicalDevices)
            {
                VulkanDebugManager::LogDeviceProperties(device);
            }
#endif // DEBUG_VULKAN


            return vk::PhysicalDevice();
        }
    }
}