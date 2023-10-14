//module;
//#include <vulkan/vulkan.hpp>
//module GuelderEngine.Vulkan;
//import :StagingBuffer;
//
//import :IBuffer;
//import :QueueFamilyIndices;
//import :DeviceManager;
//
//namespace GuelderEngine::Vulkan::Buffers
//{
//    template<typename T>
//    StagingBuffer<T>::StagingBuffer(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const std::vector<T>& data)
//    {
//        m_Size = sizeof(data[0]) * data.size();
//        if(m_Size)
//        {
//            vk::BufferCreateInfo info{vk::BufferCreateFlagBits(),
//                m_Size,
//                vk::BufferUsageFlagBits::eTransferSrc
//            };
//
//            if(indices.GetGraphicsFamily() != indices.GetTransferFamily())
//            {
//                const uint uniqueIndices[] = { indices.GetGraphicsFamily(), indices.GetTransferFamily() };
//                info.queueFamilyIndexCount = 2;
//                info.pQueueFamilyIndices = uniqueIndices;
//                info.sharingMode = vk::SharingMode::eConcurrent;
//            }
//            else
//                info.sharingMode = vk::SharingMode::eExclusive;
//
//            m_Buffer = device.createBuffer(info);
//
//            const auto memRequirements = device.getBufferMemoryRequirements(m_Buffer);
//
//            const vk::MemoryAllocateInfo allocInfo{
//                memRequirements.size,
//                    DeviceManager::FindMemType(
//                        physicalDevice,
//                        memRequirements.memoryTypeBits,
//                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
//                    )
//            };
//
//            m_BufferMemory = device.allocateMemory(allocInfo);
//            device.bindBufferMemory(m_Buffer, m_BufferMemory, 0);
//
//            void* memdata = device.mapMemory(m_BufferMemory, 0, info.size);
//            memcpy(memdata, data.data(), info.size);
//            device.unmapMemory(m_BufferMemory);
//        }
//    }
//    template<typename T>
//    StagingBuffer<T>::StagingBuffer(const StagingBuffer<T>& other)
//    {
//        m_Buffer = other.m_Buffer;
//        m_BufferMemory = other.m_BufferMemory;
//        m_Size = other.m_Size;
//    }
//    template<typename T>
//    StagingBuffer<T>& StagingBuffer<T>::operator=(const StagingBuffer<T>& other)
//    {
//        if(this == &other)
//            return *this;
//
//        m_Buffer = other.m_Buffer;
//        m_BufferMemory = other.m_BufferMemory;
//        m_Size = other.m_Size;
//
//        return *this;
//    }
//    template<typename T>
//    StagingBuffer<T>::StagingBuffer(StagingBuffer<T>&& other) noexcept
//    {
//        m_Buffer = other.m_Buffer;
//        m_BufferMemory = other.m_BufferMemory;
//        m_Size = other.m_Size;
//
//        other.Reset();
//    }
//    template<typename T>
//    StagingBuffer<T>& StagingBuffer<T>::operator=(StagingBuffer<T>&& other) noexcept
//    {
//        m_Buffer = other.m_Buffer;
//        m_BufferMemory = other.m_BufferMemory;
//        m_Size = other.m_Size;
//
//        other.Reset();
//
//        return *this;
//    }
//}