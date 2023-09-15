module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
export module GuelderEngine.Vulkan:Mesh;

import :IVulkanObject;
import GuelderEngine.Core.Types;

import <array>;
import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct Vertex;
    //temp
    using Vertices = std::vector<Vertex>;
    using Indices = std::vector<Types::uint>;

    enum class VertexFormat
    {
        Vec3Float = vk::Format::eR32G32B32Sfloat,
        Vec2Float = vk::Format::eR32G32Sfloat
    };

    struct VertexBindingInfo
    {
        VertexFormat format;
        Types::uint location;//layout(location = 1)
    };
    struct VertexAttributeDescriptionsInfo
    {
        VertexBindingInfo position;
        VertexBindingInfo color;
    };
    vk::Format ConvertVkFormat(const VertexFormat& format);
    struct Vertex : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(Vertex);

        Vertex(const glm::vec2& pos, const glm::vec3& color);

        void Reset() noexcept override;

        static vk::VertexInputBindingDescription GetBindingDescription();//idk about param
        static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescriptions(const VertexAttributeDescriptionsInfo& info = {
            {VertexFormat::Vec2Float, 0},
            {VertexFormat::Vec3Float, 1}
        });

        glm::vec2 pos;
        glm::vec3 color;
    };
    class Mesh : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(Mesh);

        Mesh(const Vertices& vertices, const Indices& indices = {});

        void Reset() noexcept override;

        const Vertices& GetVertices() const noexcept;
        const Indices& GetIndices() const noexcept;
    private:
        Vertices m_Vertices;
        Indices m_Indices;
    };
}