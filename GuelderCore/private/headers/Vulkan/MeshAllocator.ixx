module;
#include "../../headers/Core/GObject/GClass.hpp"
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:MeshAllocator;

import GuelderEngine.Core.Types;
import :VertexBuffer;
import :IndexBuffer;
import :Mesh;
import :Manager;

import <vector>;
import <algorithm>;

import GuelderEngine.Layers;

export namespace GuelderEngine::Vulkan
{
    struct MeshSector
    {
        uint starts = 0, ends = 0;

        uint GetSize() const { return ends - starts + 1; }

        bool IsComplete() const { return starts != ends; }
        void Reset()
        {
            starts = 0;
            ends = 0;
        }

        operator bool() const { return IsComplete(); }
    };
    struct FullMeshSector
    {
        WeakPtr<MeshSector> vertex;
        WeakPtr<MeshSector> index;
    };

    template<uint _dimension>
    class MeshAllocator;

    using MeshAllocator2D = MeshAllocator<2>;
    using MeshAllocator3D = MeshAllocator<3>;

    template<uint _dimension>
    class MeshAllocator
    {
    private:
        using Vertices = Vertices<_dimension>;
        using Mesh = Mesh<_dimension>;
        using Vertex = Vertex<_dimension>;
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(MeshAllocator);
        DECLARE_DEFAULT_COPYING(MeshAllocator);
        DECLARE_DEFAULT_MOVING(MeshAllocator);

        void SetMesh(const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue,
            SharedPtr<MeshSector>& mVertices,
            const Vertices& vertices,
            SharedPtr<MeshSector>& mIndices = nullptr,
            const Indices& indices = {}
            )
        {
            //deleting
            if(mVertices->IsComplete())
            {
                uint verticesDeletingSize = mVertices->ends - mVertices->starts + 1;

                auto foundToDelete = std::find_if(m_MeshSectors.begin(), m_MeshSectors.end(), [&mVertices, &mIndices](FullMeshSector& input) {return input.vertex.lock().get() == mVertices.get() && input.index.lock().get() == mIndices.get(); });

                uint currentPos = m_MeshSectors.data() - std::addressof(*foundToDelete);

                if(m_MeshSectors.size() > currentPos)
                {
                    for(uint i = currentPos + 1; i < m_MeshSectors.size(); i++)
                    {
                        auto& current = m_MeshSectors[i];

                        current.vertex.lock()->starts -= verticesDeletingSize;
                        current.vertex.lock()->ends -= verticesDeletingSize;
                    }
                    if(foundToDelete->index.lock().get())
                    {
                        uint indicesDeletingSize = mIndices->ends - mIndices->starts + 1;
                        for(uint i = currentPos + 1; i < m_MeshSectors.size(); i++)
                        {
                            auto& current = m_MeshSectors[i];

                            current.index.lock()->starts -= indicesDeletingSize;
                            current.index.lock()->ends -= indicesDeletingSize;
                        }
                    }
                }

                m_MeshSectors.erase(foundToDelete);
                m_Vertices.erase(m_Vertices.begin() + mVertices->starts, m_Vertices.begin() + mVertices->ends + 1);

                mVertices->Reset();

                if(mIndices)
                {
                    m_Indices.erase(m_Indices.begin() + mIndices->starts, m_Indices.begin() + mIndices->ends + 1);
                    mIndices->Reset();
                }
            }

            //allocation
            {
                uint _verticesStart = m_Vertices.end() - m_Vertices.begin();
                m_Vertices.insert(m_Vertices.begin() + _verticesStart, vertices.begin(), vertices.end());

                *mVertices = { _verticesStart, _verticesStart + uint(vertices.size() - 1) };

                RecreateVertexBuffer(device, physicalDevice, queueFamilyIndices, transferPool, transferQueue);

                if(!indices.empty())
                {
                    uint _indicesStart = m_Indices.end() - m_Indices.begin();
                    m_Indices.insert(m_Indices.begin() + _indicesStart, indices.begin(), indices.end());
                    *mIndices = { _indicesStart, _indicesStart + uint(indices.size() - 1) };
                    RecreateIndexBuffer(device, physicalDevice, queueFamilyIndices, transferPool, transferQueue);
                }

                auto toPush = FullMeshSector{ mVertices, mIndices };

                m_MeshSectors.push_back(toPush);
            }
        }
        void AllocateMesh(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue,
            SharedPtr<MeshSector>& mVertices,
            const Vertices& vertices,
            SharedPtr<MeshSector>& mIndices = nullptr,
            const Indices& indices = {}
        )
        {
            uint _verticesStart = m_Vertices.end() - m_Vertices.begin();
            m_Vertices.insert(m_Vertices.begin() + _verticesStart, vertices.begin(), vertices.end());

            *mVertices = { _verticesStart, _verticesStart + uint(vertices.size() - 1) };

            RecreateVertexBuffer(device, physicalDevice, queueFamilyIndices, transferPool, transferQueue);

            if(!indices.empty())
            {
                uint _indicesStart = m_Indices.end() - m_Indices.begin();
                m_Indices.insert(m_Indices.begin() + _indicesStart, indices.begin(), indices.end());
                *mIndices = { _indicesStart, _indicesStart + uint(indices.size() - 1) };
                RecreateIndexBuffer(device, physicalDevice, queueFamilyIndices, transferPool, transferQueue);
            }

            auto toPush = FullMeshSector{ mVertices, mIndices };

            m_MeshSectors.push_back(toPush);
        }
        void DeleteMesh(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue,
            SharedPtr<MeshSector>& vertices,
            SharedPtr<MeshSector>& indices = nullptr
        )
        {
            if(vertices->IsComplete())
            {
                uint verticesDeletingSize = vertices->ends - vertices->starts;

                auto foundToDelete = std::find_if(m_MeshSectors.begin(), m_MeshSectors.end(), [&vertices, &indices](FullMeshSector& input) {return input.vertex.lock().get() == vertices.get() && input.index.lock().get() == indices.get(); });

                uint currentPos = m_MeshSectors.data() - std::addressof(*foundToDelete);

                if(m_MeshSectors.size()-1 > currentPos)
                {
                    for(uint i = currentPos + 1; i < m_MeshSectors.size(); i++)
                    {
                        auto& current = m_MeshSectors[i];

                        current.vertex.lock()->starts -= verticesDeletingSize+1;
                        current.vertex.lock()->ends -= verticesDeletingSize+1;
                    }
                    if(foundToDelete->index.lock())
                    {
                        uint indicesDeletingSize = indices->ends - indices->starts;
                        for(uint i = currentPos + 1; i < m_MeshSectors.size(); i++)
                        {
                            auto& current = m_MeshSectors[i];

                            current.index.lock()->starts -= indicesDeletingSize+1;
                            current.index.lock()->ends -= indicesDeletingSize+1;
                        }
                    }
                }
                m_MeshSectors.erase(foundToDelete);

                m_Vertices.erase(m_Vertices.begin() + vertices->starts, m_Vertices.begin() + vertices->ends+1);

                vertices->Reset();

                if(indices)
                {
                    m_Indices.erase(m_Indices.begin() + indices->starts, m_Indices.begin() + indices->ends+1);
                    RecreateIndexBuffer(device, physicalDevice, queueFamilyIndices, transferPool, transferQueue);
                    indices->Reset();
                }
                RecreateVertexBuffer(device, physicalDevice, queueFamilyIndices, transferPool, transferQueue);
            }
        }

        void RecreateVertexBuffer(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue
        )
        {
            auto prevBuffer = (m_VertexBuffer);
            m_VertexBuffer = Buffers::VertexBuffer(device, physicalDevice, queueFamilyIndices, transferPool, transferQueue, m_Vertices.data(), sizeof(Vertex), m_Vertices.size());
            auto& queues = Vulkan::VulkanManager::Get().GetDevice().GetQueues();
            prevBuffer.Cleanup(device, { queues.graphics, queues.present, queues.transfer });
        }
        void RecreateIndexBuffer(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue
        )
        {
            auto prevBuffer = std::move(m_IndexBuffer);
            m_IndexBuffer = Buffers::IndexBuffer(device, physicalDevice, queueFamilyIndices, transferPool, transferQueue, m_Indices);
            auto& queues = Vulkan::VulkanManager::Get().GetDevice().GetQueues();
            prevBuffer.Cleanup(device, { queues.graphics, queues.present, queues.transfer });
        }

        Mesh GetMesh(const SharedPtr<MeshSector>& vertices, const SharedPtr<MeshSector>& indices = {}) const
        {
            Mesh result;

            if(vertices->IsComplete())
                for(auto it = m_Vertices.begin() + vertices->starts; it != m_Vertices.begin() + vertices->ends+1; ++it)
                    result.m_Vertices.push_back(*it);

            if(indices->IsComplete())
                for(auto it = m_Indices.begin() + indices->starts; it != m_Indices.begin() + indices->ends+1; ++it)
                    result.m_Indices.push_back(*it);

            return result;
        }

        void Cleanup(const vk::Device& device) const noexcept
        {
            m_VertexBuffer.Cleanup(device);
            m_IndexBuffer.Cleanup(device);
        }

        const Buffers::VertexBuffer& GetVertexBuffer() const { return m_VertexBuffer; }
        const Buffers::IndexBuffer& GetIndexBuffer() const { return m_IndexBuffer; }
        const Vertices& GetVertices() const { return m_Vertices; }
        const Indices& GetIndices() const { return m_Indices; }
    private:
        Buffers::VertexBuffer m_VertexBuffer;
        Buffers::IndexBuffer m_IndexBuffer;

        Vertices m_Vertices;
        Indices m_Indices;

        std::vector<FullMeshSector> m_MeshSectors;
    };
}