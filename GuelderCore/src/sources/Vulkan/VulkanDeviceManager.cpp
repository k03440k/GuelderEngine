module;
#include <../includes/GuelderEngine/Utils/Debug.hpp>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :VulkanDeviceManager;

import :VulkanDebugManager;
import :VulkanSwapchain;
import :VulkanPipeline;
import GuelderEngine.Core.Types;

import <optional>;
import <set>;
import <ranges>;

namespace GuelderEngine::Vulkan
{
    VulkanDeviceManager::VulkanDeviceManager(const vk::Instance& instance, GLFWwindow* glfwWindow, const std::string_view& vertPath, const std::string_view& fragPath, const std::vector<const char*>& extensions)
    {
        VkSurfaceKHR cStyle;
        GE_CORE_CLASS_ASSERT(glfwCreateWindowSurface(instance, glfwWindow, nullptr, &cStyle) == VK_SUCCESS,
            "cannot abstract GLFWwindow for vulkan surface");
        m_Surface = cStyle;

        m_PhysicalDevice = ChoosePhysicalDevice(instance, extensions);
        m_QueueIndices = VulkanQueueFamilyIndices(m_PhysicalDevice, m_Surface);
        m_Device = CreateDevice(m_PhysicalDevice, m_QueueIndices, extensions);

        //m_Queues.graphicsQueue = GetGraphicsQueue(m_Device, m_QueueIndices);
        //m_Queues.presentQueue = GetPresentQueue(m_Device, m_QueueIndices);

        int width{}, height{};
        glfwGetWindowSize(glfwWindow, &width, &height);

        //m_Swapchain = VulkanSwapchain(m_Device, m_PhysicalDevice, m_Surface, width, height, m_QueueIndices);

        m_Pipeline = VulkanPipeline(m_Device, m_PhysicalDevice, m_Surface, width, height, m_QueueIndices,
            /* m_Swapchain.m_Extent, m_Swapchain.m_Format,*/ vertPath, fragPath);
    }
    VulkanDeviceManager::VulkanDeviceManager(const VulkanDeviceManager& other)
    {
        m_PhysicalDevice = other.m_PhysicalDevice;
        m_Device = other.m_Device;
        m_QueueIndices = other.m_QueueIndices;
        m_Surface = other.m_Surface;
        //m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        //m_Queues.presentQueue = other.m_Queues.presentQueue;
        //m_Swapchain = other.m_Swapchain;
        m_Pipeline = other.m_Pipeline;
    }
    VulkanDeviceManager::VulkanDeviceManager(VulkanDeviceManager&& other) noexcept
    {
        m_PhysicalDevice = other.m_PhysicalDevice;
        m_Device = other.m_Device;
        m_QueueIndices = other.m_QueueIndices;
        m_Surface = other.m_Surface;
        //m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        //m_Queues.presentQueue = other.m_Queues.presentQueue;
        //m_Swapchain = other.m_Swapchain;
        m_Pipeline = std::forward<VulkanPipeline>(other.m_Pipeline);

        other.Reset();
    }
    VulkanDeviceManager& VulkanDeviceManager::operator=(const VulkanDeviceManager& other)
    {
        if(this == &other)
            return *this;

        m_PhysicalDevice = other.m_PhysicalDevice;
        m_Device = other.m_Device;
        m_QueueIndices = other.m_QueueIndices;
        m_Surface = other.m_Surface;
        //m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        //m_Queues.presentQueue = other.m_Queues.presentQueue;
        //m_Swapchain = other.m_Swapchain;
        m_Pipeline = other.m_Pipeline;

        return *this;
    }
    VulkanDeviceManager& VulkanDeviceManager::operator=(VulkanDeviceManager&& other) noexcept
    {
        m_PhysicalDevice = other.m_PhysicalDevice;
        m_Device = other.m_Device;
        m_QueueIndices = other.m_QueueIndices;
        m_Surface = other.m_Surface;
        //m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        //m_Queues.presentQueue = other.m_Queues.presentQueue;
        //m_Swapchain = other.m_Swapchain;
        m_Pipeline = std::forward<VulkanPipeline>(other.m_Pipeline);

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void VulkanDeviceManager::Reset() noexcept
    {
        m_PhysicalDevice = nullptr;
        m_Device = nullptr;
        m_QueueIndices = {};
        m_Surface = nullptr;
        //m_Queues.presentQueue = nullptr;
        //m_Queues.graphicsQueue = nullptr;
        //m_Swapchain.Reset();
        m_Pipeline.Reset();
    }
    void VulkanDeviceManager::Cleanup(const vk::Instance& instance) const noexcept
    {
        m_Device.waitIdle();
        m_Pipeline.Cleanup(m_Device);
        //m_Swapchain.Cleanup(m_Device);
        m_Device.destroy();
        instance.destroySurfaceKHR(m_Surface);
    }
    void VulkanDeviceManager::Render(GLFWwindow* glfwWindow, const VulkanScene& scene)
    {
        int width = 0, height = 0;
        while(width == 0 || height == 0) {
            glfwGetFramebufferSize(glfwWindow, &width, &height);
            glfwWaitEvents();
        }

        m_Pipeline.Render(m_Device, m_PhysicalDevice, m_Surface, {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}, m_QueueIndices, scene);
    }
    bool VulkanDeviceManager::CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& requestedExtensions)
    {
        std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());

        //GE_LOG(VulkanCore, Info, "Device can support following extensions:");

        for (auto&& extension : physicalDevice.enumerateDeviceExtensionProperties())
        {
            //GE_LOG(VulkanCore, Info, '\t', extension.extensionName);

            //remove this from the list of required extensions (set checks for equality automatically)
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
    bool VulkanDeviceManager::IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& extensions)
    {
        std::vector requestedExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        if(!extensions.empty())
            std::ranges::copy(extensions, std::back_inserter(requestedExtensions));

        if (!CheckDeviceExtensionsSupport(physicalDevice, requestedExtensions))
        {
            //Debug::LogError("Device cannot support requested extensions");
            return false;
        }

        return true;
    }
    vk::PhysicalDevice VulkanDeviceManager::ChoosePhysicalDevice(const vk::Instance& instance, const std::vector<const char*>& extensions)
    {
        const std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

        GE_CORE_CLASS_ASSERT(physicalDevices.size() > 0, "there are no physical devices");

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "There are ", physicalDevices.size(), " detected physical devices:");
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
            if (theBiggestDeviceMemorySize < memoryOfWholeDevice && IsDeviceSuitable(physicalDevices[i], extensions))
            {
                theBiggestDeviceMemorySize = memoryOfWholeDevice;
                idxToDeviceOftheBiggestMemory = i;
            }
        }

        GE_CORE_CLASS_ASSERT(theBiggestDeviceMemorySize > 0, "cannot choose device");

        return physicalDevices[idxToDeviceOftheBiggestMemory];
    }
    vk::Device VulkanDeviceManager::CreateDevice(const vk::PhysicalDevice& physicalDevice, const VulkanQueueFamilyIndices& indices, const std::vector<const char*>& extensions)
    {
        constexpr float queuePriority = 1.0f;

        std::vector uniqueIndices{indices.graphicsFamily.value()};
        if (indices.graphicsFamily.value() != indices.presentFamily.value())
            uniqueIndices.push_back(indices.presentFamily.value());

        std::vector<vk::DeviceQueueCreateInfo> queueDeviceInfo;
        for (auto&& queueFamilyIndex : uniqueIndices)
        {
            queueDeviceInfo.push_back(
                vk::DeviceQueueCreateInfo(
                vk::DeviceQueueCreateFlags(),
                queueFamilyIndex,
                1,
                &queuePriority));
        }

        std::vector<const char*> deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        deviceExtensions.reserve(deviceExtensions.size() + extensions.size());

        if(!extensions.empty())
            std::ranges::copy(extensions, std::back_inserter(deviceExtensions));

        const auto deviceFeatures = physicalDevice.getFeatures();//mine is .getFeatures()

        std::vector<const char*> enabledLayers;
#ifdef GE_DEBUG_VULKAN
        enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        const auto deviceInfo = vk::DeviceCreateInfo(
            vk::DeviceCreateFlags(),
            queueDeviceInfo.size(),
            queueDeviceInfo.data(),
            enabledLayers.size(),
            enabledLayers.data(),
            deviceExtensions.size(),
            deviceExtensions.data(),
            &deviceFeatures
        );

        return physicalDevice.createDevice(deviceInfo);
    }
    /*vk::Queue VulkanDeviceManager::GetGraphicsQueue(const vk::Device& device, const VulkanQueueFamilyIndices& indices) noexcept
    {
        return device.getQueue(indices.graphicsFamily.value(), 0);
    }
    vk::Queue VulkanDeviceManager::GetPresentQueue(const vk::Device& device, const VulkanQueueFamilyIndices& indices) noexcept
    {
        return device.getQueue(indices.presentFamily.value(), 0);
    }*/
}