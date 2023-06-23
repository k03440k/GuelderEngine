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
import <set>;

namespace GuelderEngine::Vulkan
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

#ifdef GE_DEBUG_VULKAN
        m_DebugManager = Vulkan::VulkanDebugManager(m_Instance, m_DLDI);
#endif // GE_DEBUG_VULKAN
        m_DeviceManager = VulkanDeviceManager(m_Instance);
    }
    vk::Instance VulkanManager::CreateVkInstance(const char* const name)
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
    //    bool VulkanManager::IsValidationLayersSupported(const std::vector<const char*>& layers)
    //    {
    //        GE_CORE_CLASS_ASSERT(layers.size() > 0, "layers size is zero");
    //
    //        std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
    //
    //#ifdef GE_DEBUG_VULKAN
    //        Logger::Log(LogLevel::Info, "Device can support following layers:");
    //        for (const auto& layer : supportedLayers)
    //        {
    //            Logger::Log(LogLevel::Info, '\t', layer.layerName);
    //        }
    //#endif // GE_DEBUG_VULKAN
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
    //#ifdef GE_DEBUG_VULKAN
    //                    Logger::Log(LogLevel::Info, "Layer \"", extension, "\" is supported");
    //#endif //GE_DEBUG_VULKAN
    //                }
    //            }
    //            if (!found)
    //            {
    //#ifdef GE_DEBUG_VULKAN
    //                Logger::Log(LogLevel::Info, "Layer \"", extension, "\" is not supported");
    //#endif //GE_DEBUG_VULKAN
    //                return false;
    //            }
    //        }
    //
    //        return true;
    //    }
    void VulkanManager::Cleanup()
    {
        m_DeviceManager.Cleanup();
#ifdef GE_DEBUG_VULKAN
        m_Instance.destroyDebugUtilsMessengerEXT(m_DebugManager.m_DebugMessenger, nullptr, m_DLDI);
#endif // GE_DEBUG_VULKAN
        m_Instance.destroy();
    }
    VulkanManager::~VulkanManager()
    {
        Cleanup();
    }
#pragma endregion
#pragma region VulkanDeviceManager
    QueueFamilyIndices::QueueFamilyIndices()
        : graphicsFamily(0), presentFamily(0)
    {}

    VulkanDeviceManager::VulkanDeviceManager(const vk::Instance& instance)
    {
        m_PhysicalDevice = ChoosePhysicalDevice(instance);
        m_QueueIndices = FindQueueFamily(m_PhysicalDevice);
        m_Device = CreateDevice(m_PhysicalDevice, m_QueueIndices);
        m_GraphicsQueue = GetQueue(m_PhysicalDevice, m_Device, m_QueueIndices);
    }
    void VulkanDeviceManager::Cleanup() const
    {
        m_Device.destroy();
    }
    VulkanDeviceManager& VulkanDeviceManager::operator=(const VulkanDeviceManager& other)
    {
        m_PhysicalDevice = other.m_PhysicalDevice;
        m_Device = other.m_Device;
        return *this;
    }
    QueueFamilyIndices VulkanDeviceManager::FindQueueFamily(const vk::PhysicalDevice& device)
    {
        QueueFamilyIndices indices;

        const std::vector queueFamilies = device.getQueueFamilyProperties();

#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("Device can support ", queueFamilies.size(), " Queue Families");
#endif

        for (size_t i = 0; i < queueFamilies.size(); i++)
        {
            if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
            {
                indices.graphicsFamily = i;
                indices.presentFamily = i;

#ifdef GE_DEBUG_VULKAN
                Debug::LogInfo("Queue Family at index ", i, " is suitable for graphics and presenting");
#endif
            }
            if (indices.IsComplete())
                break;
        }

        GE_CORE_CLASS_ASSERT(indices.IsComplete(), "Cannot find Queue Family");

        return indices;
    }
    bool VulkanDeviceManager::CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& requestedExtensions)
    {
        std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());
#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("Device can support following extensions:");
#endif

        for(auto&& extension : physicalDevice.enumerateDeviceExtensionProperties())
        {
#ifdef GE_DEBUG_VULKAN
            Debug::LogInfo('\t', extension.extensionName);
#endif
            //remove this from the list of required extensions (set checks for equality automatically)
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
    bool VulkanDeviceManager::IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice)
    {
        if(const std::vector requestedExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
            !CheckDeviceExtensionsSupport(physicalDevice, requestedExtensions))
        {
            //Debug::LogError("Device cannot support requested extensions");
            return false;
        }

        return true;
    }
    vk::PhysicalDevice VulkanDeviceManager::ChoosePhysicalDevice(const vk::Instance& instance)
    {
        const std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

        GE_CORE_CLASS_ASSERT(physicalDevices.size() > 0, "there are no physical devices");

#ifdef GE_DEBUG_VULKAN
        Debug::LogInfo("There are ", physicalDevices.size(), " detected physical devices:");
        for (const auto& device : physicalDevices)
        {
            VulkanDebugManager::LogDeviceProperties(device);
        }
#endif // GE_DEBUG_VULKAN
        //device choosing

        double theBiggestDeviceMemorySize{};
        Types::uint idxToDeviceOftheBiggestMemory{};
        //takes the most powerful device
        for (Types::uint i = 0; i < physicalDevices.size(); i++)
        {
            const vk::PhysicalDeviceMemoryProperties& memoryProperties = physicalDevices[i].getMemoryProperties();

            const uint32_t heapCount = memoryProperties.memoryHeapCount;

            double memoryOfWholeDevice{};

            // Iterate over memory heaps to find the heap with the desired memory type
            for (uint32_t heapIndex = 0; heapIndex < heapCount; ++heapIndex) {
                // Check if the memory heap is for device local memory (typically used for graphics memory)
                if (const auto& heap = memoryProperties.memoryHeaps[heapIndex]; heap.flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
                    // Convert the size to gigabytes
                    memoryOfWholeDevice += static_cast<double>(heap.size) / (1024 * 1024 * 1024);
                }
            }
            if (theBiggestDeviceMemorySize < memoryOfWholeDevice && IsDeviceSuitable(physicalDevices[i]))
            {
                theBiggestDeviceMemorySize = memoryOfWholeDevice;
                idxToDeviceOftheBiggestMemory = i;
            }
        }

        GE_CORE_CLASS_ASSERT(theBiggestDeviceMemorySize > 0, "cannot choose device");

        return physicalDevices[idxToDeviceOftheBiggestMemory];//TODO: Queue Families at the video time of 1:04:11
    }
    vk::Device VulkanDeviceManager::CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices)
    {
        constexpr float queuePriority = 1.0f;

        const auto queueDeviceInfo = vk::DeviceQueueCreateInfo(
            vk::DeviceQueueCreateFlags(), 
            indices.graphicsFamily.value(),
            1,
            &queuePriority
        );

        const auto deviceFeatures = physicalDevice.getFeatures();//

        std::vector<const char*> enabledLayers;
#ifdef GE_DEBUG_VULKAN
        enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        const auto deviceInfo = vk::DeviceCreateInfo(
            vk::DeviceCreateFlags(),
            1,
            &queueDeviceInfo,
            enabledLayers.size(),
            enabledLayers.data(),
            0,
            nullptr,
            &deviceFeatures
        );

        return physicalDevice.createDevice(deviceInfo);
    }
    vk::Queue VulkanDeviceManager::GetQueue(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const QueueFamilyIndices& indices)
    {
        return device.getQueue(indices.graphicsFamily.value(), 0);
    }
#pragma endregion
}