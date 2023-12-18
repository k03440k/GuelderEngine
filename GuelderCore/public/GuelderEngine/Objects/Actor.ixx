module;
#include <vulkan/vulkan.hpp>
//#include <glm/glm.hpp>
#include <compare>//wtf?!
//#include "../../../private/headers/Core/GObject/GClass.hpp"
//#include "../Utils/Debug.hpp"
export module GuelderEngine.Game:Actor;

import :GObjectBase;
import GuelderEngine.Vulkan;

import <memory>;

export namespace GuelderEngine
{
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
    //with buffers
    class RenderActor : public Actor
    {
    public:
        RenderActor() = default;
        virtual ~RenderActor() = default;

        RenderActor(const Vulkan::Buffers::VertexBuffer& vertexBuffer, const Vulkan::Buffers::IndexBuffer& indexBuffer);

        void Cleanup(const vk::Device& device) const noexcept;

        bool IsComplete() const noexcept;

        Vulkan::Buffers::VertexBuffer vertexBuffer;
        Vulkan::Buffers::IndexBuffer indexBuffer;
    private:
    };
}