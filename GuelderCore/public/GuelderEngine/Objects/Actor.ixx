module;
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <compare>//wtf?!
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "GuelderEngine/Utils/Debug.hpp"
//#include "../Utils/Debug.hpp"
export module GuelderEngine.Game:Actor;

import :GObjectBase;
import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;

import <memory>;

export namespace GuelderEngine
{
    template<uint, typename = float>
    class RenderActor;

    using Actor2D = RenderActor<2>;
    using Actor3D = RenderActor<3, Vector3>;

    //it is insufficient for memory, because we allocate a memory with pointer and then with vector. right now I don't know what to choose else
    template<class _Actor>
    std::shared_ptr<_Actor> MakeActor(const _Actor&& actor)
    {
        return std::make_shared<_Actor>(actor);
    }
    
    class Actor : public GObjectBase
    {
    public:
        Actor() = default;
        virtual ~Actor() = default;

        virtual void Begin() {}
        virtual void Update() {}
    private:
    };

    template<uint dimension, typename RotationType = float>
    struct RenderActorTransform
    {
    public:
        using Vec = Vector<dimension>;
    public:
        DECLARE_DEFAULT_COPY(RenderActorTransform);
        DECLARE_DEFAULT_MOVING(RenderActorTransform);
        
        RenderActorTransform() = default;
        RenderActorTransform(const Vec& translation, const Vec& scale, const Vec& position, const RotationType& rotation)
            : translation(translation), scale(scale), position(position), rotation(rotation) {}
        ~RenderActorTransform() = default;

        Vec translation;
        Vec scale;
        Vec position;
        RotationType rotation;
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
    public:
        RenderActor() = default;
        virtual ~RenderActor() = default;

        RenderActor(const Vulkan::Buffers::VertexBuffer<dimension>& _vertexBuffer, const Vulkan::Buffers::IndexBuffer& _indexBuffer)
        {
            vertexBuffer = _vertexBuffer;
            indexBuffer = _indexBuffer;
        }
        RenderActor(const Transform& transform)
            : transform(transform) {}

        void Cleanup(const vk::Device& device) const noexcept
        {
            vertexBuffer.Cleanup(device);
            indexBuffer.Cleanup(device);
        }

        virtual bool IsComplete() const noexcept { return vertexBuffer.GetVerticesCount(); }

        Vulkan::Buffers::VertexBuffer<dimension> vertexBuffer;
        Vulkan::Buffers::IndexBuffer indexBuffer;

        Transform transform;
    private:
    };
}