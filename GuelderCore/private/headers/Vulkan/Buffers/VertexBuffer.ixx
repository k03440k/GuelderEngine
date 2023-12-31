module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VertexBuffer;

//import :IBuffer;
//import :QueueFamilyIndices;
//import :Mesh;
//import GuelderEngine.Core.Types;
import :IBuffer;
import :StagingBuffer;
import :QueueFamilyIndices;
import :Mesh;
//import :DeviceManager;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan::Buffers
{
    template<uint>
    class VertexBuffer;

    using VertexBuffer2D = VertexBuffer<2>;
    using VertexBuffer3D = VertexBuffer<3>;

    template<uint dimension>
    class VertexBuffer final : public IBuffer
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(VertexBuffer);
        //DECLARE_DEFAULT_COPY(VertexBuffer);

        VertexBuffer(const VertexBuffer& other)
        {
            m_Buffer = other.m_Buffer;
            m_BufferMemory = other.m_BufferMemory;
            m_VerticesCount = other.m_VerticesCount;
            m_Size = other.m_Size;
        }
        VertexBuffer& operator=(const VertexBuffer& other)
        {
            if(this == &other)
                return *this;

            m_Buffer = other.m_Buffer;
            m_BufferMemory = other.m_BufferMemory;
            m_VerticesCount = other.m_VerticesCount;
            m_Size = other.m_Size;

            return *this;
        }
        VertexBuffer(VertexBuffer&& other) noexcept
        {
            m_Buffer = other.m_Buffer;
            m_BufferMemory = other.m_BufferMemory;
            m_VerticesCount = other.m_VerticesCount;
            m_Size = other.m_Size;

            other.Reset();
        }
        VertexBuffer& operator=(VertexBuffer&& other) noexcept
        {
            m_Buffer = other.m_Buffer;
            m_BufferMemory = other.m_BufferMemory;
            m_VerticesCount = other.m_VerticesCount;
            m_Size = other.m_Size;

            other.Reset();

            return *this;
        }

        VertexBuffer(
            const vk::Device& device, 
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue,
            const Vertices<dimension>& mesh
        )

            : m_VerticesCount(mesh.size())
        {
            m_Size = sizeof(mesh[0]) * mesh.size();
            if(m_VerticesCount)
            {
                vk::BufferCreateInfo info{ vk::BufferCreateFlagBits(),
                    sizeof(mesh[0])* mesh.size(),
                    vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst
                };

                if(queueFamilyIndices.GetGraphicsFamily() != queueFamilyIndices.GetTransferFamily())
                {
                    const uint uniqueIndices[] = { queueFamilyIndices.GetGraphicsFamily(), queueFamilyIndices.GetTransferFamily() };
                    info.queueFamilyIndexCount = 2;
                    info.pQueueFamilyIndices = uniqueIndices;
                    info.sharingMode = vk::SharingMode::eConcurrent;
                }
                else
                    info.sharingMode = vk::SharingMode::eExclusive;

                m_Buffer = device.createBuffer(info);

                const auto memRequirements = device.getBufferMemoryRequirements(m_Buffer);

                vk::MemoryAllocateInfo allocInfo{
                    memRequirements.size,
                        IBuffer::FindMemoryType(
                            physicalDevice,
                            memRequirements.memoryTypeBits,
                            vk::MemoryPropertyFlagBits::eDeviceLocal
                        )
                };

                m_BufferMemory = device.allocateMemory(allocInfo);
                device.bindBufferMemory(m_Buffer, m_BufferMemory, 0);

                const auto stagingBuffer = StagingBuffer(device, physicalDevice, queueFamilyIndices, mesh);
                CopyBuffer(stagingBuffer.GetBuffer(), m_Buffer, m_Size, device, transferPool, transferQueue);
                stagingBuffer.Cleanup(device);
            }
        }

        void Reset() noexcept override
        {
            m_Buffer = nullptr;
            m_BufferMemory = nullptr;
            m_VerticesCount = 0;
            m_Size = 0;
        }

        void Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const
        {
            cmdBuffer.bindVertexBuffers(0, 1, &m_Buffer, &offset);
        }

        uint GetVerticesCount() const noexcept { return m_VerticesCount; }
            
    private:
        uint m_VerticesCount;
    };
}