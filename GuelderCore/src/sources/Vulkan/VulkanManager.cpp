#include "../../headers/Vulkan/VulkanManager.hpp"

#include <glfw/glfw3.h>

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
            //m_DebugMessenger = CreateDebugMessenger();
        }
        vk::Instance VulkanManager::CreateVkInstance(const char* const name)
        {
            uint version{};
            vkEnumerateInstanceVersion(&version);
#ifdef DEBUG_VULKAN
            Logger::Log(LogLevel::Info, Logger::Format("System can support Vulkan variant: ", VK_API_VERSION_VARIANT(version), ", ",
                VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version)));
#endif // DEBUG_VULKAN

            vk::ApplicationInfo appInfo(name, 0, "Guelder Engine", version);

            uint glfwExtensionsCount{};
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

#ifdef DEBUG_VULKAN
            extensions.push_back("VK_EXT_debug_utils");

            if (extensions.size() > 0)
            {
                Logger::Log(LogLevel::Info, "All available extensions for Vulkan are:");
                for (const auto extension : extensions)
                {
                    Logger::Log(LogLevel::Info, extension);
                }
            }
#endif // DEBUG_VULKAN

            GE_CORE_ASSERT(IsExtensionsSupported(extensions), "extensions are not supported");

#ifdef DEBUG_VULKAN
            //m_DebugManager = VulkanDebugManager(m_Instance, m_DLDI, { "VK_LAYER_KHRONOS_validation" });

            //std::vector<const char*> layers;
            //layers.push_back("VK_LAYER_KHRONOS_validation");
            //GE_CORE_ASSERT(IsValidationLayersSupported(layers), "validation layers are not supported");

            Vulkan::VulkanDebugLayersManager layers({ "VK_LAYER_KHRONOS_validation" });

            vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
                &appInfo,
                (uint)layers.GetLayers().size(), layers.GetLayers().data(),//debug layers
                (uint)extensions.size(), extensions.data());//extensions
#else
            vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
                &appInfo,
                0, nullptr),//debug layers
                (uint)extensions.size(), extensions.data());//extensions
#endif // DEBUG_VULKAN
                return vk::createInstance(createInfo);
        }
        /*VulkanDebugManager::VulkanDebugManager(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi,
            const std::vector<const char* const>& validationLayers) : m_ValidationLayers(validationLayers)
        {
            m_DebugMessenger = CreateDebugMessenger(instance, dldi);
            GE_CORE_ASSERT(AreValidationLayersSupported(validationLayers), "layers are not supported");
        }*/
        bool VulkanManager::IsExtensionsSupported(const std::vector<const char*>& extensions)
        {
            GE_CORE_ASSERT(extensions.size() > 0, "extensions size is zero");

            std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

#ifdef DEBUG_VULKAN
            Logger::Log(LogLevel::Info, "Device can support following extensions:");
            for (const auto& supportedExtension : supportedExtensions)
            {
                Logger::Log(LogLevel::Info, '\t', supportedExtension.extensionName);
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
                        Logger::Log(LogLevel::Info, "Extension \"", extension, "\" is supported");
#endif //DEBUG_VULKAN
                    }
                }
                if (!found)
                {
#ifdef DEBUG_VULKAN
                    Logger::Log(LogLevel::Info, "Extension \"", extension, "\" is not supported");
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
        void VulkanManager::Cleanup()
        {
            m_Instance.destroy();
        }
        VulkanManager::~VulkanManager()
        {
            Cleanup();
        }
#pragma endregion
    }
}