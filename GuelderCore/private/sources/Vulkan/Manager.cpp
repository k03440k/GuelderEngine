module;
#include "../../../includes/GuelderEngine/Utils/Debug.hpp"
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
import GuelderEngine.Core.Types;
import GuelderEngine.Core;

import <vector>;
import <string_view>;
import <set>;

using namespace GuelderEngine::Types;

namespace GuelderEngine::Vulkan
{
    Scene::Scene()
    {
        Types::uint reserve{};

        for(float x = -1.0f; x < 1.0f; x += 0.5f)
            for(float y = -1.0f; y < 1.0f; y += 0.5f)
                reserve++;

        m_TrianglePositions.reserve(reserve);

        for (float x =  -1.0f; x < 1.0f; x += 0.2f)
            for (float y = -1.0f; y < 1.0f; y += 0.2f)
                m_TrianglePositions.push_back(glm::vec3(x, y, 0.0f));
    }
    Scene::Scene(const Scene& other)
    {
        m_TrianglePositions = other.m_TrianglePositions;
    }
    Scene::Scene(Scene&& other) noexcept
    {
        m_TrianglePositions = other.m_TrianglePositions;

        other.Reset();
    }
    Scene& Scene::operator=(const Scene& other)
    {
        if(this == &other)
            return *this;

        m_TrianglePositions = other.m_TrianglePositions;

        return *this;
    }
    Scene& Scene::operator=(Scene&& other) noexcept
    {
        m_TrianglePositions = other.m_TrianglePositions;

        other.Reset();

        return *this;
    }
    void Scene::Reset() noexcept
    {
        m_TrianglePositions.clear();
    }
}
namespace GuelderEngine::Vulkan
{
    VulkanManager::VulkanManager(GLFWwindow* glfwWindow, const Types::uint& width, const Types::uint& height, const std::string_view& vertPath, const std::string_view& fragPath, const Mesh_t& mesh,
        const std::string_view& name)
    {
        m_Instance = CreateVkInstance(name.data());
        m_DeviceManager = DeviceManager(m_Instance, glfwWindow, { width, height }, vertPath, fragPath);

#ifdef GE_DEBUG_VULKAN
        m_DebugManager = DebugManager(m_Instance);
#endif // GE_DEBUG_VULKAN
        m_Pipeline = Pipeline(
            m_DeviceManager.GetDevice(), 
            m_DeviceManager.GetPhysicalDevice(), 
            m_DeviceManager.GetSurface(),
            {width, height}, 
            m_DeviceManager.GetQueueIndices(),
            vertPath, 
            fragPath,
            mesh
        );
    }
    VulkanManager::VulkanManager(const VulkanManager& other)
    {
        m_Instance = other.m_Instance;
        m_Pipeline = other.m_Pipeline;
        m_DeviceManager = other.m_DeviceManager;
        m_Scene = other.m_Scene;
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = other.m_DebugManager;
#endif //GE_DEBUG_VULKAN
    }
    VulkanManager::VulkanManager(VulkanManager&& other) noexcept
    {
        m_Instance = other.m_Instance;
        m_Pipeline = std::forward<Pipeline>(other.m_Pipeline);
        m_DeviceManager = std::forward<DeviceManager>(other.m_DeviceManager);
        m_Scene = std::forward<Scene>(other.m_Scene);
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = std::forward<DebugManager>(other.m_DebugManager);
#endif //GE_DEBUG_VULKAN

        other.Reset();
    }
    VulkanManager& VulkanManager::operator=(const VulkanManager& other)
    {
        if(this == &other)
            return *this;

        m_Instance = other.m_Instance;
        m_Pipeline = other.m_Pipeline;
        m_DeviceManager = other.m_DeviceManager;
        m_Scene = other.m_Scene;
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = other.m_DebugManager;
#endif //GE_DEBUG_VULKAN

        return *this;
    }
    VulkanManager& VulkanManager::operator=(VulkanManager&& other) noexcept
    {
        m_Instance = other.m_Instance;
        m_Pipeline = std::forward<Pipeline>(other.m_Pipeline);
        m_DeviceManager = std::forward<DeviceManager>(other.m_DeviceManager);
        m_Scene = std::forward<Scene>(other.m_Scene);
#ifdef GE_DEBUG_VULKAN
        m_DebugManager = std::forward<DebugManager>(other.m_DebugManager);
#endif //GE_DEBUG_VULKAN

        other.Reset();
        return *this;
    }
    VulkanManager::~VulkanManager()
    {
        Cleanup();
    }

    void VulkanManager::Render(Types::uint width, Types::uint height)
    {
        m_Pipeline.Render(
            m_DeviceManager.GetDevice(), 
            m_DeviceManager.GetPhysicalDevice(),
            m_DeviceManager.GetSurface(), 
            {width, height},
            m_DeviceManager.GetQueueIndices(),
            m_Scene
        );
    }

    vk::Instance VulkanManager::CreateVkInstance(const char* name)
    {
        Types::uint version{};
        GE_CORE_CLASS_ASSERT(vk::enumerateInstanceVersion(&version) == vk::Result::eSuccess, "cannot enumerateInstanceVersion");

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "System must support Vulkan version: ",
            VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version), ", variant: ", VK_API_VERSION_VARIANT(version));
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

        const Vulkan::DebugLayersManager layers({ "VK_LAYER_KHRONOS_validation" });

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
    void VulkanManager::SetMesh(const Mesh& mesh)
    {
        m_Pipeline.SetMesh(m_DeviceManager.GetDevice(), m_DeviceManager.GetPhysicalDevice(), m_DeviceManager.GetQueueIndices(), mesh);
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
        m_Pipeline.Reset();
        m_DeviceManager.Reset();
#ifdef GE_DEBUG_VULKAN
        m_DebugManager.Reset();
#endif
    }
    void VulkanManager::Cleanup() const noexcept
    {
        m_DeviceManager.WaitIdle();
        m_Pipeline.Cleanup(m_DeviceManager.GetDevice());
        m_DeviceManager.Cleanup(m_Instance);
#ifdef GE_DEBUG_VULKAN
        //m_Instance.destroyDebugUtilsMessengerEXT(m_DebugManager.m_DebugMessenger, nullptr, m_DLDI);
        m_DebugManager.Cleanup(m_Instance);
#endif // GE_DEBUG_VULKAN
        m_Instance.destroy();
    }
}