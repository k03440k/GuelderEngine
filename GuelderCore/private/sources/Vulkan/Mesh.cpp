module;
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
module GuelderEngine.Vulkan;
import :Mesh;

#pragma region Vertex
namespace GuelderEngine::Vulkan
{
    Vertex::Vertex(const Vertex& other)
    {
        pos = other.pos;
        color = other.color;
    }
    Vertex& Vertex::operator=(const Vertex& other)
    {
        if(this == &other)
            return *this;

        pos = other.pos;
        color = other.color;

        return *this;
    }
    Vertex::Vertex(Vertex&& other) noexcept
    {
        pos = other.pos;
        color = other.color;

        other.Reset();
    }
    Vertex& Vertex::operator=(Vertex&& other) noexcept
    {
        pos = other.pos;
        color = other.color;

        other.Reset();

        return *this;
    }

}
namespace GuelderEngine::Vulkan
{
    Vertex::Vertex(const glm::vec2& pos, const glm::vec3& color)
        : pos(pos), color(color)
    {
    }
    void Vertex::Reset() noexcept
    {
        pos = glm::vec2();
        color = glm::vec3();
    }
}
#pragma endregion

namespace GuelderEngine::Vulkan
{
    
}
namespace GuelderEngine::Vulkan
{
    vk::VertexInputBindingDescription Vertex::GetBindingDescription()
    {
        constexpr vk::VertexInputBindingDescription description{0, sizeof(Vertex), vk::VertexInputRate::eVertex};

        return description;
    }
    std::array<vk::VertexInputAttributeDescription, 2> Vertex::GetAttributeDescriptions(const Types::uint& inPosLocation, const Types::uint& inColorLocation)
    {
        std::array<vk::VertexInputAttributeDescription, 2> descriptions;

        //pos
        descriptions[0].binding = 0;
        descriptions[0].location = inPosLocation;
        descriptions[0].format = vk::Format::eR32G32Sfloat;
        descriptions[0].offset = offsetof(Vertex, pos);

        //color
        descriptions[1].binding = 0;
        descriptions[1].location = inColorLocation;
        descriptions[1].format = vk::Format::eR32G32B32Sfloat;
        descriptions[1].offset = offsetof(Vertex, color);

        return descriptions;
    }
}