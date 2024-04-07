module;
#include <vulkan/vulkan.hpp>
#include "../Utils/Debug.hpp"
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
        MeshComponent(const Mesh& mesh = {})
        {
            SetMesh(mesh);
        }
        /*MeshComponent(MeshComponent&& other)
        {
            m_VertexBuffer = std::move(other.m_VertexBuffer);
            m_IndexBuffer = std::move(other.m_IndexBuffer);
            m_Mesh = std::move(other.m_Mesh);
        }
        MeshComponent& operator=(MeshComponent&& other)
        {

        }*/
        ~MeshComponent()
        {
            Vulkan::VulkanManager::Get()->GetDevice().WaitIdle();
                m_VertexBuffer.Cleanup(Vulkan::VulkanManager::Get()->GetDevice().GetDevice());

                m_IndexBuffer.Cleanup(Vulkan::VulkanManager::Get()->GetDevice().GetDevice());
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

        //TODO: make only one vertex buffer to boost fps
        void SetMesh(const Mesh& mesh)
        {
            auto pastVB = m_VertexBuffer;
            auto pastIB = m_IndexBuffer;

            m_Mesh = mesh;

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
            
            m_IndexBuffer = IndexBuffer
            (
                Vulkan::VulkanManager::Get()->GetDevice().GetDevice(),
                Vulkan::VulkanManager::Get()->GetDevice().GetPhysicalDevice(),
                Vulkan::VulkanManager::Get()->GetDevice().GetQueueIndices(),
                Vulkan::VulkanManager::Get()->GetDevice().GetCommandPoolTransfer().GetCommandPool(),
                Vulkan::VulkanManager::Get()->GetDevice().GetQueues().transfer,
                mesh.GetIndices()
            );

            pastVB.Cleanup(Vulkan::VulkanManager::Get()->GetDevice().GetDevice());
            pastIB.Cleanup(Vulkan::VulkanManager::Get()->GetDevice().GetDevice());
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