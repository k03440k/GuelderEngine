module;
#include <vulkan/vulkan.hpp>
#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :VertexBuffer;

import :DeviceManager;
import :IVulkanObject;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    //Buffer::Buffer(
    //    const vk::Device& device, 
    //    const vk::PhysicalDevice& physicalDevice, 
    //    const QueueFamilyIndices& indices,
    //    const Types::uint& size, 
    //    const vk::BufferUsageFlags& usageProperties,
    //    const vk::MemoryPropertyFlags& memoryProperties
    //)
    //{
    //    if(size)
    //    {
    //        vk::BufferCreateInfo info{
    //            vk::BufferCreateFlagBits(),
    //                size,
    //                usageProperties
    //        };
    //        if(indices.GetGraphicsFamily() != indices.GetTransferFamily())
    //        {
    //            const Types::uint uniqueIndices[] = { indices.GetGraphicsFamily(), indices.GetTransferFamily() };
    //            info.sharingMode = vk::SharingMode::eConcurrent;
    //            info.queueFamilyIndexCount = 2;
    //            info.pQueueFamilyIndices = uniqueIndices;
    //        }
    //        else
    //            info.sharingMode = vk::SharingMode::eExclusive;

    //        m_Buffer = device.createBuffer(info);

    //        const auto memRequirements = device.getBufferMemoryRequirements(m_Buffer);

    //        const vk::MemoryAllocateInfo allocInfo{
    //            memRequirements.size,
    //                DeviceManager::FindMemType(
    //                    physicalDevice,
    //                    memRequirements.memoryTypeBits,
    //                    memoryProperties
    //                )
    //        };

    //        m_BufferMemory = device.allocateMemory(allocInfo);
    //        device.bindBufferMemory(m_Buffer, m_BufferMemory, 0);

    //        /*void* data = device.mapMemory(m_BufferMemory, 0, info.size);
    //        memcpy(data, mesh.data(), info.size);
    //        device.unmapMemory(m_BufferMemory);*/
    //    }
    //}
    //Buffer::Buffer(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const Mesh_t& mesh)
    //{
    //    
    //}
    void IBuffer::Reset() noexcept
    {
        m_Buffer = nullptr;
        m_BufferMemory = nullptr;
    }
    void IBuffer::Cleanup(const vk::Device& device) const noexcept
    {
        device.freeMemory(m_BufferMemory);
        device.destroyBuffer(m_Buffer);
    }
    /*void Buffer::Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const
    {
        cmdBuffer.bindVertexBuffers(0, 1, &m_Buffer, &offset);
    }*/
}