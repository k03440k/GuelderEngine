module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:Buffer;

import GuelderEngine.Core.Types;
import :QueueFamilyIndices;
import :CommandPool;
import :Mesh;

export namespace GuelderEngine::Vulkan::Buffers
{
    class Buffer;

    void CopyBuffer(
        const vk::Buffer& srcBuffer,
        const vk::Buffer& dstBuffer,
        const vk::DeviceSize& size,
        const vk::Device& device,
        const vk::CommandPool& transferPool,
        const vk::Queue& transferQueue
    );

    class Buffer : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(Buffer);
        DECLARE_DEFAULT_COPYING(Buffer);
        DECLARE_MOVING(Buffer);

        Buffer(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::BufferUsageFlags& usageFlags,
            const vk::MemoryPropertyFlags& memoryProperty,
            const uint& instanceSize,
            const uint& instanceCount,
            const uint& minOffsetAlignment = 1
        );

        void Reset() noexcept override;
        virtual void Cleanup(const vk::Device& device) const noexcept;
        //virtual void Cleanup(const vk::Device& device, const std::vector<vk::Queue>& queuesToWait) const noexcept;

        void WriteToBuffer(const void* data, const vk::DeviceSize& size = VK_WHOLE_SIZE, const vk::DeviceSize& offset = 0) const;
        void WriteToIndex(const void* data, const uint& index) const;

        void Flush(const vk::Device& device, const vk::DeviceSize& size = VK_WHOLE_SIZE, const vk::DeviceSize& offset = 0) const;
        void FlushIndex(const vk::Device& device, const uint& index) const;
        void Invalidate(const vk::Device& device, const vk::DeviceSize& size = VK_WHOLE_SIZE, const vk::DeviceSize& offset = 0) const;
        void InvalidateIndex(const vk::Device& device, const uint& index) const;

        void MapMemory(const vk::Device& device, const vk::DeviceSize& size = VK_WHOLE_SIZE, const vk::DeviceSize& offset = 0);
        void UnmapMemory(const vk::Device& device) const;

        static uint FindMemoryType(const vk::PhysicalDevice& physicalDevice, const uint& typeFilter, const vk::MemoryPropertyFlags& properties);

        bool IsInitialized() const noexcept;

        const vk::Buffer& GetBuffer() const noexcept;
        const vk::DeviceMemory& GetBufferMemory() const noexcept;
        vk::DeviceSize GetSize() const noexcept;
        vk::DescriptorBufferInfo GetDescriptorBufferInfo(const vk::DeviceSize& size = VK_WHOLE_SIZE, const vk::DeviceSize& offset = 0) const noexcept;
        vk::DescriptorBufferInfo GetDescriptorBufferInfoForIndex(const uint& index) const noexcept;
        vk::BufferUsageFlags GetBufferUsageFlags() const noexcept;
        vk::MemoryPropertyFlags GetMemoryPropertyFlags() const noexcept;
        uint GetInstanceCount() const noexcept;
        uint GetInstanceSize() const noexcept;
        uint GetAlignmentSize() const noexcept;
        const void* GetMappedMemory() const noexcept;

    protected:
        static uint GetAlignment(const uint& instanceSize, const uint& minOffsetAlignment);

        vk::Buffer m_Buffer;
        vk::DeviceMemory m_BufferMemory;
        vk::DeviceSize m_Size;

        void* m_MappedMemory;
        uint m_InstanceSize;
        uint m_InstanceCount;
        uint m_AlignmentSize;
        vk::BufferUsageFlags m_UsageFlags;
        vk::MemoryPropertyFlags m_MemoryPropertyFlags;
    };
}