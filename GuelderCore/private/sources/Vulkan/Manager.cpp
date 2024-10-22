module;
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
module GuelderEngine.Vulkan;
import :Manager;

import :DebugManager;
import :DeviceManager;
import :ShaderManager;
import :Pipeline;
import GuelderEngine.Debug;
import GuelderEngine.Core;
import GuelderEngine.Core.Types;
import GuelderEngine.Utils;

import <vector>;
import <string_view>;
import <set>;

namespace GuelderEngine::Vulkan
{
    VulkanManager::VulkanManager(const std::string_view& name)
        : m_Instance(CreateVkInstance(name.data()))
    {
        m_DeviceManager = DeviceManager(m_Instance, { VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME });

#ifdef GE_DEBUG_VULKAN
        m_DebugManager = DebugManager(m_Instance);
#endif // GE_DEBUG_VULKAN
    }
    VulkanManager::VulkanManager(VulkanManager&& other) noexcept
    {
        m_Instance = other.m_Instance;
        m_DeviceManager = std::forward<DeviceManager>(other.m_DeviceManager);
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = std::forward<DebugManager>(other.m_DebugManager);
#endif //GE_DEBUG_VULKAN

        other.Reset();
    }
    VulkanManager& VulkanManager::operator=(VulkanManager&& other) noexcept
    {
        m_Instance = other.m_Instance;
        m_DeviceManager = std::forward<DeviceManager>(other.m_DeviceManager);
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = std::forward<DebugManager>(other.m_DebugManager);
#endif //GE_DEBUG_VULKAN

        other.Reset();
        return *this;
    }
    VulkanManager::~VulkanManager()
    {
        VulkanManager::Cleanup();
    }
}
namespace GuelderEngine::Vulkan
{
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
        m_DeviceManager.WaitIdle();
        m_DeviceManager.Cleanup();
#ifdef GE_DEBUG_VULKAN
        m_DebugManager.Cleanup(m_Instance);
#endif // GE_DEBUG_VULKAN
        m_Instance.destroy();
    }

    vk::Instance VulkanManager::CreateVkInstance(const char* name)
    {
        uint version{};
        GE_ASSERT(vk::enumerateInstanceVersion(&version) == vk::Result::eSuccess, "cannot enumerateInstanceVersion");

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "System must support Vulkan version: ",
            VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version), ", variant: ", VK_API_VERSION_VARIANT(version));
#endif // GE_DEBUG_VULKAN

        vk::ApplicationInfo appInfo(name, VK_MAKE_VERSION(0, 0, 1), "Guelder Engine", VK_MAKE_VERSION(0, 0, 1), VK_API_VERSION_1_3);

        Window::InitGLFW();

        uint glfwExtensionsCount{};
        const char** const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);
        extensions.reserve(extensions.size() + 2);
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

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

        GE_ASSERT(AreExtensionsSupported(extensions), "extensions are not supported");

#ifdef GE_DEBUG_VULKAN

        const std::vector<const char*> layers{ "VK_LAYER_KHRONOS_validation" };
        GE_ASSERT(AreValidationLayersSupported(layers), "validation layers are not supported");

        vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
            &appInfo,
            static_cast<uint32_t>(layers.size()), layers.data(),//debug layers
            static_cast<uint32_t>(extensions.size()), extensions.data());//extensions
#else
        vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
            &appInfo,
            0, nullptr,//debug layers
            (uint)extensions.size(), extensions.data());//extensions
#endif // GE_DEBUG_VULKAN

        return vk::createInstance(createInfo);
    }
    bool VulkanManager::AreExtensionsSupported(const std::vector<const char*>& extensions)
    {
        GE_ASSERT(extensions.size() > 0, "extensions size is zero");

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
                if(strcmp(extension, supportedExtension.extensionName) == 0)
                    found = true;

            if(!found)
                return false;
        }

        return true;
    }
}
namespace GuelderEngine::Vulkan
{
    const Vulkan::VulkanManager& VulkanManager::Get()
    {
        return GEApplication::GetVulkanManager();
    }
    const DeviceManager& VulkanManager::GetDeviceManager() const
    {
        return m_DeviceManager;
    }
    const vk::Instance& VulkanManager::GetInstance() const
    {
        return m_Instance;
    }
}