module;
#include <vulkan/vulkan.hpp>
#include "../Utils/Debug.hpp"
export module GuelderEngine.Actors:MeshComponent;

import <functional>;

import :ActorComponent;
import GuelderEngine.Vulkan;
import GuelderEngine.Core.Types;

import <memory>;

//TODO: possibility of allocating huge chunk of vertices in one method, so to save time on allocation of all actors and their meshes

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
        MeshComponent() = default;
        MeshComponent(const Mesh& mesh)
        {
            m_Vertex = std::make_shared<Vulkan::MeshSector>(0, 0);
            m_Index = std::make_shared<Vulkan::MeshSector>(0, 0);
            AllocateNewMesh(mesh);
        }

        void Reset()
        {
            m_Vertex.Reset();
            m_Index.Reset();
        }

        void SetMesh(const Mesh& mesh)
        {
            m_MeshAllocator.SetMesh(
                Vulkan::VulkanManager::Get().GetDeviceManager().GetDevice(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetPhysicalDevice(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetQueueIndices(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetCommandPoolTransfer().GetCommandPool(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetQueues().transfer,
                m_Vertex,
                mesh.GetVertices(),
                m_Index,
                mesh.GetIndices()
            );
        }

        Mesh GetMesh() const
        {
            return m_MeshAllocator.GetMesh(m_Vertex, m_Index);
        }

        bool IsComplete() const 
        { 
            return m_Vertex->IsComplete();
        }

        static const VertexBuffer& GetVertexBuffer() { return m_MeshAllocator.GetVertexBuffer(); }
        static const IndexBuffer& GetIndexBuffer() { return m_MeshAllocator.GetIndexBuffer(); }

        const SharedPtr<Vulkan::MeshSector>& GetVertexSector() const { return m_Vertex; }
        const SharedPtr<Vulkan::MeshSector>& GetIndexSector() const { return m_Index; }

        static const Vulkan::MeshAllocator<dimension>& GetMeshAllocator() { return m_MeshAllocator; }
    private:
        friend class GEApplication;
        static Vulkan::MeshAllocator<dimension> m_MeshAllocator;

        void AllocateNewMesh(const Mesh& mesh)
        {
            m_MeshAllocator.AllocateMesh
            (
                Vulkan::VulkanManager::Get().GetDeviceManager().GetDevice(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetPhysicalDevice(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetQueueIndices(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetCommandPoolTransfer().GetCommandPool(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetQueues().transfer,
                m_Vertex,
                mesh.GetVertices(),
                m_Index,
                mesh.GetIndices()
            );
        }
        void DeleteMesh()
        {
            m_MeshAllocator.DeleteMesh
            (
                Vulkan::VulkanManager::Get().GetDeviceManager().GetDevice(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetPhysicalDevice(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetQueueIndices(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetCommandPoolTransfer().GetCommandPool(),
                Vulkan::VulkanManager::Get().GetDeviceManager().GetQueues().transfer,
                m_Vertex,
                m_Index
            );
        }

        SharedPtr<Vulkan::MeshSector> m_Vertex;
        SharedPtr<Vulkan::MeshSector> m_Index;
    };

    Vulkan::MeshAllocator<2> MeshComponent<2>::m_MeshAllocator = Vulkan::MeshAllocator<2>{};
    Vulkan::MeshAllocator<3> MeshComponent<3>::m_MeshAllocator = Vulkan::MeshAllocator<3>{};
}