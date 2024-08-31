module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:DeviceManager;

import :IVulkanObject;
import :DebugManager;
import :QueueFamilyIndices;
import :Swapchain;
import :Pipeline;
import :VertexBuffer;
import GuelderEngine.Core.Types;

import <optional>;
import :CommandPool;

export namespace GuelderEngine::Vulkan
{
    struct Queues
    {
        vk::Queue graphics;
        vk::Queue present;
        vk::Queue transfer;
    };

    /**
     * @brief You must manually call .Cleanup() func instead of dtor
    */
    class DeviceManager final : public IVulkanObject//, INHERIT_GClass(DeviceManager)
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(DeviceManager);
        DECLARE_DEFAULT_COPYING(DeviceManager);
        DECLARE_MOVING(DeviceManager);

        DeviceManager(const vk::Instance& instance, const std::vector<const char*>& extensions = {});

        void Reset() noexcept override;
        void Cleanup() const noexcept;

        //void Render(uint width, uint height, const Scene& scene);

        static uint FindMemType(const vk::PhysicalDevice& physicalDevice, const uint& typeFilter, const vk::MemoryPropertyFlags& properties);
        static vk::Format FindSupportedFormat(const vk::PhysicalDevice& physicalDevice, const std::vector<vk::Format>& formats, const vk::ImageTiling& imageTiling, const vk::FormatFeatureFlagBits& features);

        template<uint dimension>
        Buffers::VertexBuffer MakeVertexBuffer(const Vertices<dimension>& vertices) const
        {
            return Buffers::VertexBuffer(m_Device, m_PhysicalDevice, m_QueueIndices, m_CommandPoolTransfer.GetCommandPool(), m_Queues.transfer, vertices.data(), sizeof(vertices[0]), vertices.size());
        }
        Buffers::IndexBuffer MakeIndexBuffer(const Indices& indices) const;

        void WaitIdle() const noexcept;

        const vk::Device& GetDevice() const;
        const vk::PhysicalDevice& GetPhysicalDevice() const;
        const QueueFamilyIndices& GetQueueIndices() const;

        const CommandPool& GetCommandPool() const;
        const CommandPool& GetCommandPoolTransfer() const;
        const Queues& GetQueues() const;

        static vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance& instance, const std::vector<const char*>& extensions = {});
    private:
        static bool CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice,
            const std::vector<const char*>& requestedExtensions);
        static bool IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& extensions = {});
        static vk::Device CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const std::vector<const char*>& extensions = {});

        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;
        QueueFamilyIndices m_QueueIndices;

        CommandPool m_CommandPool;
        CommandPool m_CommandPoolTransfer;
        Queues m_Queues;
    };
}