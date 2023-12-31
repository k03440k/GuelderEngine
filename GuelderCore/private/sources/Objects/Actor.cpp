//module;
////#include <vulkan/vulkan.hpp>
////#include <glm/glm.hpp>
//#include <compare>//wtf?
//module GuelderEngine.Game;
//import :Actor;
//
//import :GObjectBase;
//import GuelderEngine.Vulkan;
//
//namespace GuelderEngine
//{
//    using namespace Vulkan::Buffers;
//    RenderActor::RenderActor(const VertexBuffer& _vertexBuffer, const IndexBuffer& _indexBuffer)
//    {
//        vertexBuffer = _vertexBuffer;
//        indexBuffer = _indexBuffer;
//    }
//    void RenderActor::Cleanup(const vk::Device& device) const noexcept
//    {
//        vertexBuffer.Cleanup(device);
//        indexBuffer.Cleanup(device);
//    }
//    bool RenderActor::IsComplete() const noexcept
//    {
//        return vertexBuffer.GetVerticesCount();
//    }
//}