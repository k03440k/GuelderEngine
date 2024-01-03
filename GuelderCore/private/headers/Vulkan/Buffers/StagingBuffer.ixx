module;
#include <cstring>//for memcpy
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:StagingBuffer;

import :IBuffer;
import :Mesh;
//import :DeviceManager;
import :QueueFamilyIndices;

export namespace GuelderEngine::Vulkan::Buffers
{
    template<typename T>
    class StagingBuffer : public IBuffer
    {
    public:
        //DECLARE_DCAD_AND_CAM(StagingBuffer);

        StagingBuffer(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const std::vector<T>& data)
        {
            m_Size = sizeof(data[0]) * data.size();
            if(m_Size)
            {
                vk::BufferCreateInfo info{vk::BufferCreateFlagBits(),
                    m_Size,
                    vk::BufferUsageFlagBits::eTransferSrc
                };

                if(indices.GetGraphicsFamily() != indices.GetTransferFamily())
                {
                    const uint uniqueIndices[] = { indices.GetGraphicsFamily(), indices.GetTransferFamily() };
                    info.queueFamilyIndexCount = 2;
                    info.pQueueFamilyIndices = uniqueIndices;
                    info.sharingMode = vk::SharingMode::eConcurrent;
                }
                else
                    info.sharingMode = vk::SharingMode::eExclusive;

                m_Buffer = device.createBuffer(info);

                const auto memRequirements = device.getBufferMemoryRequirements(m_Buffer);

                const vk::MemoryAllocateInfo allocInfo{
                    memRequirements.size,
                        IBuffer::FindMemoryType(
                            physicalDevice,
                            memRequirements.memoryTypeBits,
                            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
                        )
                };

                m_BufferMemory = device.allocateMemory(allocInfo);
                device.bindBufferMemory(m_Buffer, m_BufferMemory, 0);

                void* memdata = device.mapMemory(m_BufferMemory, 0, info.size);
                memcpy(memdata, data.data(), info.size);
                device.unmapMemory(m_BufferMemory);
            }
        }
        StagingBuffer(const StagingBuffer& other)
        {
            m_Buffer = other.m_Buffer;
            m_BufferMemory = other.m_BufferMemory;
            m_Size = other.m_Size;
        }
        StagingBuffer& operator=(const StagingBuffer& other)
        {
            if(this == &other)
                return *this;

            m_Buffer = other.m_Buffer;
            m_BufferMemory = other.m_BufferMemory;
            m_Size = other.m_Size;

            return *this;
        }
        StagingBuffer(StagingBuffer&& other) noexcept
        {
            m_Buffer = other.m_Buffer;
            m_BufferMemory = other.m_BufferMemory;
            m_Size = other.m_Size;

            other.Reset();
        }
        StagingBuffer& operator=(StagingBuffer&& other) noexcept
        {
            m_Buffer = other.m_Buffer;
            m_BufferMemory = other.m_BufferMemory;
            m_Size = other.m_Size;

            other.Reset();

            return *this;
        }
    };
}