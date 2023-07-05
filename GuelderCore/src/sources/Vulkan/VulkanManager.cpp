module;
#include "../../../includes/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
module GuelderEngine.Vulkan;
import :VulkanManager;

import :VulkanDebugManager;
import :VulkanDeviceManager;
//import :VulkanSurfaceManager;
import GuelderEngine.Debug;
import GuelderEngine.Core.Types;

import <vector>;
import <string_view>;
import <set>;

namespace GuelderEngine::Vulkan
{
    vk::Instance VulkanManager::CreateVkInstance(const char* name)
    {
        Types::uint version{};
        GE_CORE_CLASS_ASSERT(vk::enumerateInstanceVersion(&version) == vk::Result::eSuccess, "cannot enumerateInstanceVersion");

#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("System can support Vulkan variant: ", VK_API_VERSION_VARIANT(version), ", ",
                       VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version));
#endif // GE_DEBUG_VULKAN

        vk::ApplicationInfo appInfo(name, VK_MAKE_VERSION(0, 0, 1),
                                    "Guelder Engine", VK_MAKE_VERSION(0, 0, 1), VK_API_VERSION_1_3);

        Types::uint glfwExtensionsCount{};
        const char** const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);
        extensions.push_back("VK_EXT_debug_utils");

#ifdef GE_DEBUG_VULKAN
        if (extensions.size() > 0)
        {
            Debug::LogInfo("All available extensions for Vulkan are:");
            for (auto&& extension : extensions)
            {
                Debug::LogInfo(extension);
            }
        }
#endif // GE_DEBUG_VULKAN

        GE_CORE_CLASS_ASSERT(AreExtensionsSupported(extensions), "extensions are not supported");

#ifdef GE_DEBUG_VULKAN

        const Vulkan::VulkanDebugLayersManager layers({ "VK_LAYER_KHRONOS_validation" });

        vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
                                          &appInfo,
                                          static_cast<uint32_t>(layers.GetLayers().size()), layers.GetLayers().data(),//debug layers
                                          static_cast<uint32_t>(extensions.size()), extensions.data());//extensions
#else
            vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
                &appInfo,
                0, nullptr,//debug layers
                (Types::uint)extensions.size(), extensions.data());//extensions
#endif // GE_DEBUG_VULKAN
        return vk::createInstance(createInfo);
    }
    /*VulkanDebugManager::VulkanDebugManager(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi,
            const std::vector<const char* const>& validationLayers) : m_ValidationLayers(validationLayers)
        {
            m_DebugMessenger = CreateDebugMessenger(instance, dldi);
            GE_CORE_CLASS_ASSERT(AreValidationLayersSupported(validationLayers), "layers are not supported");
        }*/
    bool VulkanManager::AreExtensionsSupported(const std::vector<const char*>& extensions)
    {
        GE_CORE_CLASS_ASSERT(extensions.size() > 0, "extensions size is zero");

        const std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("Device can support following extensions:");
        for (const auto& supportedExtension : supportedExtensions)
        {
            Debug::LogInfo('\t', supportedExtension.extensionName);
        }
#endif //GE_DEBUG_VULKAN

        bool found = false;
        for (auto&& extension : extensions)
        {
            found = false;
            for (const auto& supportedExtension : supportedExtensions)
            {
                if (strcmp(extension, supportedExtension.extensionName) == 0)
                {
                    found = true;
#ifdef GE_DEBUG_VULKAN
                    Debug::LogInfo("Extension \"", extension, "\" is supported");
#endif //GE_DEBUG_VULKAN
                }
            }
            if (!found)
            {
#ifdef GE_DEBUG_VULKAN
                Debug::LogInfo("Extension \"", extension, "\" is not supported");
#endif //GE_DEBUG_VULKAN
                return false;
            }
        }

        return true;
    }
    void VulkanManager::Reset()
    {
        m_Instance = nullptr;
        m_DLDI = nullptr;
        m_DeviceManager.Reset();
#ifdef GE_DEBUG_VULKAN
        m_DebugManager.Reset();
#endif
    }
    void VulkanManager::Cleanup() const
    {
        m_DeviceManager.Cleanup(m_Instance);
#ifdef GE_DEBUG_VULKAN
        //m_Instance.destroyDebugUtilsMessengerEXT(m_DebugManager.m_DebugMessenger, nullptr, m_DLDI);
        m_DebugManager.Cleanup(m_Instance, m_DLDI);
#endif // GE_DEBUG_VULKAN
        m_Instance.destroy();
    }
    VulkanManager::VulkanManager(GLFWwindow* glfwWindow, const std::string_view& name)
    {
        m_Instance = CreateVkInstance(name.data());
        m_DLDI = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
        m_DeviceManager = VulkanDeviceManager(m_Instance, glfwWindow);

#ifdef GE_DEBUG_VULKAN
        m_DebugManager = VulkanDebugManager(m_Instance, m_DLDI);
#endif // GE_DEBUG_VULKAN
    }
    VulkanManager::VulkanManager(const VulkanManager& other)
    {
        m_Instance = other.m_Instance;
        m_DLDI = other.m_DLDI;
        m_DeviceManager = other.m_DeviceManager;
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = other.m_DebugManager;
#endif //GE_DEBUG_VULKAN
    }
    VulkanManager::VulkanManager(VulkanManager&& other)
    {
        m_Instance = other.m_Instance;
        m_DLDI = other.m_DLDI;
        m_DeviceManager = other.m_DeviceManager;
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = other.m_DebugManager;
#endif //GE_DEBUG_VULKAN

        other.m_Instance = nullptr;
        other.m_DLDI = nullptr;
        other.m_DeviceManager.Reset();
        //other.m_DeviceManager.Cleanup(m_Instance);//idk
#ifdef GE_DEBUG_VULKAN
        other.m_DebugManager.Reset();
        //other.m_DebugManager.Cleanup(m_Instance, m_DLDI);//idk
#endif
    }
    VulkanManager& VulkanManager::operator=(VulkanManager&& other)
    {
        m_Instance = other.m_Instance;
        m_DLDI = other.m_DLDI;
        m_DeviceManager = other.m_DeviceManager;
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = other.m_DebugManager;
#endif //GE_DEBUG_VULKAN

        other.m_Instance = nullptr;
        other.m_DLDI = nullptr;
        other.m_DeviceManager.Reset();
        //other.m_DeviceManager.Cleanup(m_Instance);//idk
#ifdef GE_DEBUG_VULKAN
        other.m_DebugManager.Reset();
        //other.m_DebugManager.Cleanup(m_Instance, m_DLDI);//idk
#endif
        return *this;
    }
    VulkanManager& VulkanManager::operator=(const VulkanManager& other)
    {
        if(this == &other)
            return *this;

        m_Instance = other.m_Instance;
        m_DLDI = other.m_DLDI;
        m_DeviceManager = other.m_DeviceManager;
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = other.m_DebugManager;
#endif //GE_DEBUG_VULKAN

        return *this;
    }
    VulkanManager::~VulkanManager()
    {
        Cleanup();
    }
}