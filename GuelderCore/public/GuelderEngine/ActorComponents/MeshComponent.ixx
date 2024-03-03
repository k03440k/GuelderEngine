module;
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Actors:MeshComponent;

import <functional>;

import :ActorComponent;
import GuelderEngine.Vulkan;
import GuelderEngine.Core.Types;

import <memory>;

export namespace GuelderEngine
{
    template<uint>
    class MeshComponent;

    using MeshComponent2D = MeshComponent<2>;
    using MeshComponent3D = MeshComponent<3>;

    template<uint dimension>
    class MeshComponent : public SceneComponent<dimension>
    {
    public:
        using VertexBuffer = Vulkan::Buffers::VertexBuffer;
        using IndexBuffer = Vulkan::Buffers::IndexBuffer;
        using Mesh = Vulkan::Mesh<dimension>;
    public:
        ~MeshComponent() override = default;

        MeshComponent(const Mesh& mesh = {})
        {
            SetMesh(mesh);
        }

        void Reset()
        {
            m_Mesh.Reset();
            m_VertexBuffer.Reset();
            m_IndexBuffer.Reset();
        }
        void Cleanup(const vk::Device& device) const noexcept
        {
            m_VertexBuffer.Cleanup(device);
            m_IndexBuffer.Cleanup(device);
        }

        void SetMesh(const Mesh& mesh)
        {
            m_Mesh = mesh;
            /*const vk::Device& device,
                const vk::PhysicalDevice& physicalDevice,
                const QueueFamilyIndices& queueFamilyIndices,
                const vk::CommandPool& transferPool,
                const vk::Queue& transferQueue,
                const void* data,
                const uint& sizeOfAllData*/
            m_VertexBuffer = VertexBuffer
            (
                Vulkan::VulkanManager::Get()->GetDevice().GetDevice(),
                Vulkan::VulkanManager::Get()->GetDevice().GetPhysicalDevice(),
                Vulkan::VulkanManager::Get()->GetDevice().GetQueueIndices(),
                Vulkan::VulkanManager::Get()->GetDevice().GetCommandPoolTransfer().GetCommandPool(),
                Vulkan::VulkanManager::Get()->GetDevice().GetQueues().transfer,
                mesh.GetVertices().data(),
                sizeof(mesh.GetVertices()[0]) * mesh.GetVertices().size()
            );
            /*
             const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue,
            const Indices& indices
            */
            m_IndexBuffer = IndexBuffer
            (
                Vulkan::VulkanManager::Get()->GetDevice().GetDevice(),
                Vulkan::VulkanManager::Get()->GetDevice().GetPhysicalDevice(),
                Vulkan::VulkanManager::Get()->GetDevice().GetQueueIndices(),
                Vulkan::VulkanManager::Get()->GetDevice().GetCommandPoolTransfer().GetCommandPool(),
                Vulkan::VulkanManager::Get()->GetDevice().GetQueues().transfer,
                mesh.GetIndices()
            );
        }

        const Mesh& GetMesh() const { return m_Mesh; }
        const VertexBuffer& GetVertexBuffer() const { return m_VertexBuffer; }
        const IndexBuffer& GetIndexBuffer() const { return m_IndexBuffer; }
    private:
        VertexBuffer m_VertexBuffer;
        IndexBuffer m_IndexBuffer;
        
        Mesh m_Mesh;
    };
}