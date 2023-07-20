module;
#include "../../../includes/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
module GuelderEngine.Vulkan;
import :VulkanManager;

import :VulkanDebugManager;
import :VulkanDeviceManager;
import :VulkanShaderManager;
//import :VulkanSurfaceManager;
import GuelderEngine.Debug;
import GuelderEngine.Core.Types;
import GuelderEngine.Core;

import <vector>;
import <string_view>;
import <set>;

using namespace GuelderEngine::Types;

namespace GuelderEngine::Vulkan
{
    VulkanManager::VulkanManager(GLFWwindow* glfwWindow, const std::string_view& vertPath, const std::string_view& fragPath,
        const std::string_view& name)
    {
        m_Instance = CreateVkInstance(name.data());
        m_DeviceManager = VulkanDeviceManager(m_Instance, glfwWindow, vertPath, fragPath);

#ifdef GE_DEBUG_VULKAN
        m_DebugManager = VulkanDebugManager(m_Instance);
#endif // GE_DEBUG_VULKAN
    }
    VulkanManager::VulkanManager(const VulkanManager& other)
    {
        m_Instance = other.m_Instance;
        m_DeviceManager = other.m_DeviceManager;
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = other.m_DebugManager;
#endif //GE_DEBUG_VULKAN
    }
    VulkanManager::VulkanManager(VulkanManager&& other) noexcept
    {
        m_Instance = other.m_Instance;
        m_DeviceManager = std::forward<VulkanDeviceManager>(other.m_DeviceManager);
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = std::forward<VulkanDebugManager>(other.m_DebugManager);
#endif //GE_DEBUG_VULKAN

        other.Reset();
//#ifdef GE_DEBUG_VULKAN
        //other.m_DebugManager.Cleanup(m_Instance, m_DLDI);//idk
//#endif
    }
    VulkanManager& VulkanManager::operator=(const VulkanManager& other)
    {
        if(this == &other)
            return *this;

        m_Instance = other.m_Instance;
        m_DeviceManager = other.m_DeviceManager;
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = other.m_DebugManager;
#endif //GE_DEBUG_VULKAN

        return *this;
    }
    VulkanManager& VulkanManager::operator=(VulkanManager&& other) noexcept
    {
        m_Instance = other.m_Instance;
        m_DeviceManager = std::forward<VulkanDeviceManager>(other.m_DeviceManager);
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = std::forward<VulkanDebugManager>(other.m_DebugManager);
#endif //GE_DEBUG_VULKAN

        other.Reset();
//#ifdef GE_DEBUG_VULKAN
        //other.m_DebugManager.Cleanup(m_Instance, m_DLDI);//idk
//#endif
        return *this;
    }
    VulkanManager::~VulkanManager()
    {
        Cleanup();
    }

    void VulkanManager::Render() const
    {
        m_DeviceManager.Render();
    }

    vk::Instance VulkanManager::CreateVkInstance(const char* name)
    {
        Types::uint version{};
        GE_CORE_CLASS_ASSERT(vk::enumerateInstanceVersion(&version) == vk::Result::eSuccess, "cannot enumerateInstanceVersion");

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "System can support Vulkan variant: ", VK_API_VERSION_VARIANT(version), ", ",
            VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version));
#endif // GE_DEBUG_VULKAN

        vk::ApplicationInfo appInfo(name, VK_MAKE_VERSION(0, 0, 1),
            "Guelder Engine", VK_MAKE_VERSION(0, 0, 1), VK_API_VERSION_1_3);

        Types::uint glfwExtensionsCount{};
        const char** const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);
        extensions.push_back("VK_EXT_debug_utils");

#ifdef GE_DEBUG_VULKAN
        if(extensions.size() > 0)
        {
            GE_LOG(VulkanCore, Info, "All available extensions for Vulkan are:");
            for(auto&& extension : extensions)
            {
                GE_LOG(VulkanCore, Info, extension);
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
    bool VulkanManager::AreExtensionsSupported(const std::vector<const char*>& extensions)
    {
        GE_CORE_CLASS_ASSERT(extensions.size() > 0, "extensions size is zero");

        const std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "Device can support following extensions:");
        for(const auto& supportedExtension : supportedExtensions)
        {
            GE_LOG(VulkanCore, Info, '\t', supportedExtension.extensionName);
        }
#endif //GE_DEBUG_VULKAN

        bool found = false;
        for(auto&& extension : extensions)
        {
            found = false;
            for(const auto& supportedExtension : supportedExtensions)
            {
                if(strcmp(extension, supportedExtension.extensionName) == 0)
                {
                    found = true;
#ifdef GE_DEBUG_VULKAN
                    GE_LOG(VulkanCore, Info, "Extension \"", extension, "\" is supported");
#endif //GE_DEBUG_VULKAN
                }
            }
            if(!found)
            {
#ifdef GE_DEBUG_VULKAN
                GE_LOG(VulkanCore, Info, "Extension \"", extension, "\" is not supported");
#endif //GE_DEBUG_VULKAN
                return false;
            }
        }

        return true;
    }
    /*void VulkanManager::LoadVertexShader(const std::string_view& name)
{
}
void VulkanManager::LoadFragmentShader(const std::string_view& name)
{
}*/
    void VulkanManager::Reset() noexcept
    {
        m_Instance = nullptr;
        m_DeviceManager.Reset();
#ifdef GE_DEBUG_VULKAN
        m_DebugManager.Reset();
#endif
    }
    void VulkanManager::Cleanup() const noexcept
    {
        m_DeviceManager.Cleanup(m_Instance);
#ifdef GE_DEBUG_VULKAN
        //m_Instance.destroyDebugUtilsMessengerEXT(m_DebugManager.m_DebugMessenger, nullptr, m_DLDI);
        m_DebugManager.Cleanup(m_Instance);
#endif // GE_DEBUG_VULKAN
        m_Instance.destroy();
    }
}