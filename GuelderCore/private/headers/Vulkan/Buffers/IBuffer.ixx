module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
#include "GuelderEngine/Utils/Debug.hpp"
export module GuelderEngine.Vulkan:IBuffer;

import :QueueFamilyIndices;
import :CommandPool;
import :Mesh;

export namespace GuelderEngine::Vulkan::Buffers
{
    class IBuffer;

    void CopyBuffer(
        const vk::Buffer& srcBuffer,
        const vk::Buffer& dstBuffer,
        const vk::DeviceSize& size,
        const vk::Device& device,
        const vk::CommandPool& transferPool,
        const vk::Queue& transferQueue
    );

    class IBuffer : public IVulkanObject
    {
    public:
        DEFINE_INTERFACE(IBuffer);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;
        void Cleanup(const vk::Device& device, const std::vector<vk::Queue> queuesToWait) const noexcept;

        const vk::Buffer& GetBuffer() const noexcept;
        const vk::DeviceMemory& GetBufferMemory() const noexcept;
        vk::DeviceSize GetSize() const noexcept;

        bool Initialized() const noexcept;

        static uint FindMemoryType(const vk::PhysicalDevice& physicalDevice, const uint& typeFilter, const vk::MemoryPropertyFlags& properties);

    protected:
        vk::Buffer m_Buffer;
        vk::DeviceMemory m_BufferMemory;
        vk::DeviceSize m_Size;
    };
}