module;
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
module GuelderEngine.Vulkan;
import :Mesh;

#pragma region Vertex
namespace GuelderEngine::Vulkan
{
    vk::Format ConvertVkFormat(const VertexFormat& format)
    {
        return static_cast<vk::Format>(static_cast<uint>(format));
    }
    Vertex2D::Vertex2D(const Vertex2D& other)
    {
        pos = other.pos;
        color = other.color;
    }
    Vertex2D& Vertex2D::operator=(const Vertex2D& other)
    {
        if(this == &other)
            return *this;

        pos = other.pos;
        color = other.color;

        return *this;
    }
    Vertex2D::Vertex2D(Vertex2D&& other) noexcept
    {
        pos = other.pos;
        color = other.color;

        other.Reset();
    }
    Vertex2D& Vertex2D::operator=(Vertex2D&& other) noexcept
    {
        pos = other.pos;
        color = other.color;

        other.Reset();

        return *this;
    }

}
namespace GuelderEngine::Vulkan
{
    Vertex2D::Vertex2D(const glm::vec2& pos, const glm::vec3& color)
        : pos(pos), color(color)
    {
    }
    void Vertex2D::Reset() noexcept
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
    vk::VertexInputBindingDescription Vertex2D::GetBindingDescription()
    {
        constexpr vk::VertexInputBindingDescription description{0, sizeof(Vertex2D), vk::VertexInputRate::eVertex};

        return description;
    }
    std::array<vk::VertexInputAttributeDescription, 2> Vertex2D::GetAttributeDescriptions(const VertexAttributeDescriptionsInfo& info)
    {
        std::array<vk::VertexInputAttributeDescription, 2> descriptions;

        //pos
        descriptions[0].binding = 0;
        descriptions[0].location = info.position.location;
        descriptions[0].format = ConvertVkFormat(info.position.format);
        descriptions[0].offset = offsetof(Vertex2D, pos);

        //color
        descriptions[1].binding = 0;
        descriptions[1].location = info.color.location;
        descriptions[1].format = ConvertVkFormat(info.color.format);
        descriptions[1].offset = offsetof(Vertex2D, color);

        return descriptions;
    }
}

namespace GuelderEngine::Vulkan
{
    Mesh2D::Mesh2D(const Vertices2D& vertices, const Indices& indices)
        : m_Vertices(vertices), m_Indices(indices)
    {
    }
    void Mesh2D::Reset() noexcept
    {
        m_Vertices.clear();
        m_Indices.clear();
    }
    Mesh2D::Mesh2D(const Mesh2D& other)
    {
        m_Vertices = other.m_Vertices;
        m_Indices = other.m_Indices;
    }
    Mesh2D& Mesh2D::operator=(const Mesh2D& other)
    {
        if(this == &other)
            return *this;

        m_Vertices = other.m_Vertices;
        m_Indices = other.m_Indices;

        return *this;
    }
    Mesh2D::Mesh2D(Mesh2D&& other) noexcept
    {
        m_Vertices = std::forward<Vertices2D>(other.m_Vertices);
        m_Indices = std::forward<Indices>(other.m_Indices);

        other.Reset();
    }
    Mesh2D& Mesh2D::operator=(Mesh2D&& other) noexcept
    {
        m_Vertices = std::forward<Vertices2D>(other.m_Vertices);
        m_Indices = std::forward<Indices>(other.m_Indices);

        other.Reset();

        return *this;
    }
    const Vertices2D& Mesh2D::GetVertices() const noexcept
    {
        return m_Vertices;
    }
    const Indices& Mesh2D::GetIndices() const noexcept
    {
        return m_Indices;
    }
}