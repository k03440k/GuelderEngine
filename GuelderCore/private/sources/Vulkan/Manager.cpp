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

//namespace GuelderEngine
//{
//    const std::unique_ptr<Vulkan::VulkanManager>& GetVulkanManager()
//    {
//        return GEApplication::GetVulkanManager();
//    }
//}

namespace GuelderEngine::Vulkan
{
    //vk::Instance VulkanManager::instance = VulkanManager::CreateVkInstance("GuelderEngine");
    //vk::PhysicalDevice DeviceManager::m_PhysicalDevice = DeviceManager::ChoosePhysicalDevice(VulkanManager::GetInstance());
    //DeviceManager VulkanManager::m_DeviceManager = DeviceManager(instance);

    VulkanManager::VulkanManager(const std::string_view& name)
        : m_Instance(CreateVkInstance(name.data()))/*, m_DeviceManager(m_Instance)*/
    {
        m_DeviceManager = DeviceManager(m_Instance);

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
    const std::unique_ptr<Vulkan::VulkanManager>& VulkanManager::Get()
    {
        return GEApplication::GetVulkanManager();
    }
    const DeviceManager& VulkanManager::GetDevice() const
    {
        return m_DeviceManager;
    }
    const vk::Instance& VulkanManager::GetInstance() const
    {
        return m_Instance;
    }
    vk::Instance VulkanManager::CreateVkInstance(const char* name)
    {
        uint version{};
        GE_CLASS_ASSERT(vk::enumerateInstanceVersion(&version) == vk::Result::eSuccess, "cannot enumerateInstanceVersion");

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "System must support Vulkan version: ",
            VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version), ", variant: ", VK_API_VERSION_VARIANT(version));
#endif // GE_DEBUG_VULKAN

        vk::ApplicationInfo appInfo(name, VK_MAKE_VERSION(0, 0, 1),
            "Guelder Engine", VK_MAKE_VERSION(0, 0, 1), VK_API_VERSION_1_3);

        Window::InitGLFW();

        uint glfwExtensionsCount{};
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

        GE_CLASS_ASSERT(AreExtensionsSupported(extensions), "extensions are not supported");

#ifdef GE_DEBUG_VULKAN

        const Vulkan::DebugLayersManager layers({ "VK_LAYER_KHRONOS_validation" });

        vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
            &appInfo,
            static_cast<uint32_t>(layers.GetLayers().size()), layers.GetLayers().data(),//debug layers
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
        GE_CLASS_ASSERT(extensions.size() > 0, "extensions size is zero");

        const std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

#ifdef GE_DEBUG_VULKAN
        /*GE_LOG(VulkanCore, Info, "Device can support following extensions:");
        for(const auto& supportedExtension : supportedExtensions)
        {
            GE_LOG(VulkanCore, Info, '\t', supportedExtension.extensionName);
        }*/
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

                    //GE_LOG(VulkanCore, Info, "Extension \"", extension, "\" is supported");
                }
            }
            if(!found)
            {
                //GE_LOG(VulkanCore, Info, "Extension \"", extension, "\" is not supported");

                return false;
            }
        }

        return true;
    }
    //void VulkanManager::SetMesh(const Mesh2D& mesh)
    //{
    //    //m_Pipeline.SetMesh(m_DeviceManager.GetDevice(), m_DeviceManager.GetPhysicalDevice(), m_DeviceManager.GetQueueIndices(), mesh);
    //}
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
        m_DeviceManager.WaitIdle();
        m_DeviceManager.Cleanup();
#ifdef GE_DEBUG_VULKAN
        m_DebugManager.Cleanup(m_Instance);
#endif // GE_DEBUG_VULKAN
        m_Instance.destroy();
    }
}