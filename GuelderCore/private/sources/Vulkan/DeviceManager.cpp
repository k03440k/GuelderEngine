module;
#include <../public/GuelderEngine/Utils/Debug.hpp>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :DeviceManager;

import :DebugManager;
import :Swapchain;
import :Pipeline;
import :Manager;
import :IVulkanObject;
import GuelderEngine.Core;
import GuelderEngine.Core.Types;

import <optional>;
import <set>;
import <ranges>;

namespace GuelderEngine::Vulkan
{
    //vk::PhysicalDevice DeviceManager::m_PhysicalDevice = DeviceManager::ChoosePhysicalDevice(VulkanManager::GetInstance());
    //make device static by some way

    DeviceManager::DeviceManager(const vk::Instance& instance, const std::vector<const char*>& extensions)
    {
        m_PhysicalDevice = ChoosePhysicalDevice(instance, extensions);
        m_QueueIndices = QueueFamilyIndices(m_PhysicalDevice/*, m_Surface*/);
        m_Device = CreateDevice(m_PhysicalDevice, m_QueueIndices, extensions);

        m_Queues.graphics = m_Device.getQueue(m_QueueIndices.GetGraphicsFamily(), 0);
        m_Queues.present = m_Device.getQueue(m_QueueIndices.GetPresentFamily(), 0);
        m_Queues.transfer = m_Device.getQueue(m_QueueIndices.GetTransferFamily(), 0);

        m_CommandPool = CommandPool(m_Device, m_QueueIndices, vk::QueueFlagBits::eGraphics);
        m_CommandPoolTransfer = CommandPool(m_Device, m_QueueIndices, vk::QueueFlagBits::eTransfer);
    }
    DeviceManager::DeviceManager(DeviceManager&& other) noexcept
    {
        m_Device = other.m_Device;
        m_PhysicalDevice = other.m_PhysicalDevice;
        m_QueueIndices = other.m_QueueIndices;
        m_CommandPool = std::forward<CommandPool>(other.m_CommandPool);
        m_CommandPoolTransfer = std::forward<CommandPool>(other.m_CommandPoolTransfer);
        m_Queues = other.m_Queues;

        other.Reset();
    }
    DeviceManager& DeviceManager::operator=(DeviceManager&& other) noexcept
    {
        m_Device = other.m_Device;
        m_PhysicalDevice = other.m_PhysicalDevice;
        m_QueueIndices = other.m_QueueIndices;
        m_CommandPool = std::forward<CommandPool>(other.m_CommandPool);
        m_CommandPoolTransfer = std::forward<CommandPool>(other.m_CommandPoolTransfer);
        m_Queues = other.m_Queues;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void DeviceManager::Reset() noexcept
    {
        m_PhysicalDevice = nullptr;
        m_Device = nullptr;
        m_QueueIndices = {};
        m_Queues.transfer = nullptr;
        m_Queues.present = nullptr;
        m_Queues.graphics = nullptr;
        m_CommandPool.Reset();
        m_CommandPoolTransfer.Reset();
    }
    void DeviceManager::Cleanup() const noexcept
    {
        m_CommandPool.Cleanup(m_Device);
        m_CommandPoolTransfer.Cleanup(m_Device);
        m_Device.destroy();
        //instance.destroySurfaceKHR(m_Surface);
    }
    /*uint DeviceManager::FindMemType(const vk::PhysicalDevice& physicalDevice, const uint& typeFilter, const vk::MemoryPropertyFlags& properties)
    {
        const auto memProperties = physicalDevice.getMemoryProperties();

        for (uint i = 0; i < memProperties.memoryTypeCount; ++i)
        {
            if(typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        GE_THROW("Failed to find suitable memory type");
    }*/
    vk::Format DeviceManager::FindSupportedFormat(const vk::PhysicalDevice& physicalDevice, const std::vector<vk::Format>& formats, const vk::ImageTiling& imageTiling, const vk::FormatFeatureFlagBits& features)
    {
        for(auto& format : formats)
        {
            vk::FormatProperties properties;
            physicalDevice.getFormatProperties(format, &properties);

            if(imageTiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & features) == features ||
                imageTiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & features) == features)
                return format;
        }
        GE_THROW("failed to find supported format");
    }
    Buffers::IndexBuffer DeviceManager::MakeIndexBuffer(const Indices& indices) const
    {
        return Buffers::IndexBuffer(m_Device, m_PhysicalDevice, m_QueueIndices, m_CommandPoolTransfer.GetCommandPool(), m_Queues.transfer, indices);
    }
    const vk::Device& DeviceManager::GetDevice() const
    {
        return m_Device;
    }
    const vk::PhysicalDevice& DeviceManager::GetPhysicalDevice() const
    {
        return m_PhysicalDevice;
    }
    const QueueFamilyIndices& DeviceManager::GetQueueIndices() const
    {
        return m_QueueIndices;
    }
    const CommandPool& DeviceManager::GetCommandPool() const
    {
        return m_CommandPool;
    }
    const CommandPool& DeviceManager::GetCommandPoolTransfer() const
    {
        return m_CommandPoolTransfer;
    }
    const Queues& DeviceManager::GetQueues() const
    {
        return m_Queues;
    }
    void DeviceManager::WaitIdle() const noexcept
    {
        m_Device.waitIdle();
    }
    bool DeviceManager::CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& requestedExtensions)
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
    bool DeviceManager::IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& extensions)
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
    vk::PhysicalDevice DeviceManager::ChoosePhysicalDevice(const vk::Instance& instance, const std::vector<const char*>& extensions)
    {
        const std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

        GE_ASSERT(physicalDevices.size() > 0, "there are no physical devices");

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "There are ", physicalDevices.size(), " detected physical devices:");
        for (const auto& device : physicalDevices)
        {
            DebugManager::LogDeviceProperties(device);
        }
#endif // GE_DEBUG_VULKAN

        //device choosing
        double theBiggestDeviceMemorySize{};
        uint idxToDeviceOftheBiggestMemory{};
        //takes the most powerful device
        for (uint i = 0; i < physicalDevices.size(); i++)
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

        GE_ASSERT(theBiggestDeviceMemorySize > 0, "cannot choose device");

        return physicalDevices[idxToDeviceOftheBiggestMemory];
    }
    vk::Device DeviceManager::CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const std::vector<const char*>& extensions)
    {
        constexpr float queuePriority = 1.0f;

        std::vector uniqueIndices{indices.GetGraphicsFamily()};
        if (indices.GetGraphicsFamily() != indices.GetPresentFamily())
            uniqueIndices.push_back(indices.GetPresentFamily());
        if(indices.GetGraphicsFamily() != indices.GetTransferFamily())
            uniqueIndices.push_back(indices.GetTransferFamily());

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
}