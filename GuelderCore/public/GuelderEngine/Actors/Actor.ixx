module;
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <compare>//wtf?!
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "GuelderEngine/Utils/Debug.hpp"
//#include "../Utils/Debug.hpp"
export module GuelderEngine.Actors:Actor;

import :GObject;
import :RenderActorTransform;
import :MeshComponent;
import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;

import <memory>;

export namespace GuelderEngine
{
    template<uint, typename = float>
    class RenderActor;

    using RenderActor2D = RenderActor<2>;
    using RenderActor3D = RenderActor<3, Vector3>;

    //it is insufficient for memory, because we allocate a memory with pointer and then with vector. right now I don't know what to choose else
    template<class _Actor>
    std::shared_ptr<_Actor> MakeActor(const _Actor&& actor)
    {
        return std::make_shared<_Actor>(actor);
    }
    
    class Actor : public GObject
    {
    public:
        Actor() = default;
        ~Actor() override = default;

        virtual void BeginPlay() {}
        virtual void Update() {}
    private:
    };

    
    template<uint dimension, uint matrix, typename RotateType = float>
    Mat<matrix> MatFromRenderActorTransform(const RenderActorTransform<dimension, RotateType>&)
    {
        GE_THROW("There is no a such dimension(", dimension, ')');
    }

    //with buffers
    template<uint dimension, typename RotationType = float>
    class RenderActor : public Actor
    {
    public:
        using Transform = RenderActorTransform<dimension, RotationType>;
        using MeshComponent = MeshComponent<dimension>;
        using Mesh = Vulkan::Mesh<dimension>;
    public:
        RenderActor() = default;
        ~RenderActor() override = default;

        RenderActor(const Vulkan::Buffers::VertexBuffer& vertexBuffer, const Vulkan::Buffers::IndexBuffer& indexBuffer)
            : meshComponent(vertexBuffer, indexBuffer) {}
        RenderActor(const Mesh& mesh, const Transform& transform)
            : meshComponent(mesh), transform(transform) {}

        void Cleanup(const vk::Device& device) const noexcept
        {
            meshComponent.Cleanup(device);
        }

        virtual bool IsComplete() const noexcept { return meshComponent.GetMesh().GetVertices().size() && meshComponent.GetVertexBuffer().GetSize() || !meshComponent.GetMesh().GetVertices().size() && !meshComponent.GetVertexBuffer().GetSize(); }

        MeshComponent meshComponent;

        Transform transform;
    private:
    };
}