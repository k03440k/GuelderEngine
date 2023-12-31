module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
export module GuelderEngine.Vulkan:Mesh;

import GuelderEngine.Core.Types;
import :IVulkanObject;

import <array>;
import <vector>;

export namespace GuelderEngine::Vulkan
{
    template<uint>
    struct Vertex;
    template<uint>
    class Mesh;

    using Vertex2D = Vertex<2>;
    using Vertex3D = Vertex<3>;

    using Mesh2D = Mesh<2>;
    using Mesh3D = Mesh<3>;

    template<uint dimension>
    using Vertices = std::vector<Vertex<dimension>>;
    using Vertices2D = std::vector<Vertex2D>;
    using Indices = std::vector<uint>;

    template<uint>
    struct SimplePushConstantData{};
    template<>
    struct SimplePushConstantData<2>
    {
        glm::mat2 transform{1.0f};
        glm::vec2 pos;
        alignas(16) glm::vec3 color;
    };
    template<>
    struct SimplePushConstantData<3>
    {
        glm::mat3 transform{1.0f};
        glm::vec3 pos;
        alignas(16) glm::vec3 color;//I guess that the alignas must be different
    };
    using SimplePushConstantData2D = SimplePushConstantData<2>;
    using SimplePushConstantData3D = SimplePushConstantData<3>;

    enum class VertexFormat
    {
        Vec3Float = vk::Format::eR32G32B32Sfloat,
        Vec2Float = vk::Format::eR32G32Sfloat
    };

    struct VertexBindingInfo
    {
        VertexFormat format;
        uint location;//layout(location = 1)
    };
    struct VertexAttributeDescriptionsInfo
    {
        VertexBindingInfo position;
        VertexBindingInfo color;
    };
    vk::Format ConvertVkFormat(const VertexFormat& format)
    {
        return static_cast<vk::Format>(static_cast<uint>(format));
    }
    
    template<uint _dimension>
    struct Vertex : public IVulkanObject
    {
    private:
        consteval static VertexFormat GetVertexFormat()
        {
            if constexpr(_dimension == 2)
            {
                return VertexFormat::Vec2Float;
            }
            else if constexpr(_dimension == 3)
            {
                return VertexFormat::Vec3Float;
            }
            else
            {
                GE_THROW("wrong dimension");
            }
        }
    public:
        using PositionVector = Vector<_dimension>;
    public:
        
        DECLARE_DEFAULT_CTOR_AND_DTOR(Vertex);
        DECLARE_DEFAULT_COPY(Vertex);
        DECLARE_DEFAULT_MOVING(Vertex);

        Vertex(const PositionVector& pos, const Vector3& color)
            : pos(pos), color(color) {}

        void Reset() noexcept override
        {
            pos = PositionVector();
            color = Vector3();
        }

        static vk::VertexInputBindingDescription GetBindingDescription()
        {
            constexpr vk::VertexInputBindingDescription description{0, sizeof(Vertex), vk::VertexInputRate::eVertex};

            return description;
        }
        static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescriptions
        (
            const VertexAttributeDescriptionsInfo& info = 
            {
                {GetVertexFormat(), 0},
                {VertexFormat::Vec3Float, 1}
            }
        )
        {
            std::array<vk::VertexInputAttributeDescription, 2> descriptions;

            //pos
            descriptions[0].binding = 0;
            descriptions[0].location = info.position.location;
            descriptions[0].format = ConvertVkFormat(info.position.format);
            descriptions[0].offset = offsetof(Vertex, pos);

            //color
            descriptions[1].binding = 0;
            descriptions[1].location = info.color.location;
            descriptions[1].format = ConvertVkFormat(info.color.format);
            descriptions[1].offset = offsetof(Vertex, color);

            return descriptions;
        }

        PositionVector pos;
        Vector3 color;
    };
    template<uint _dimension>
    class Mesh : public IVulkanObject
    {
    private:
        using Vertices = Vertices<_dimension>;
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(Mesh);
        DECLARE_DEFAULT_COPY(Mesh);
        DECLARE_DEFAULT_MOVING(Mesh);

        Mesh(const Vertices& vertices, const Indices& indices = {})
            : m_Vertices(vertices), m_Indices(indices) {}

        void Reset() noexcept override
        {
            m_Vertices.clear();
            m_Indices.clear();
        }

        const Vertices& GetVertices() const noexcept { return m_Vertices; }
        const Indices& GetIndices() const noexcept { return m_Indices; }
    private:
        Vertices m_Vertices;
        Indices m_Indices;
    };
}