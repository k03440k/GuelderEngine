module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
export module GuelderEngine.Vulkan:Mesh;

import GuelderEngine.Core.Types;
import :IVulkanObject;

import <array>;
import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct Vertex2D;
    //temp
    using Vertices2D = std::vector<Vertex2D>;
    using Indices = std::vector<uint>;

    //TODO: add possibility to push custom constant data
    struct SimplePushConstantData
    {
        glm::mat2 transform{1.0f};
        glm::vec2 pos;
        alignas(16) glm::vec3 color;
    };

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
    vk::Format ConvertVkFormat(const VertexFormat& format);
    struct Vertex2D : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(Vertex2D);

        Vertex2D(const glm::vec2& pos, const glm::vec3& color);

        void Reset() noexcept override;

        static vk::VertexInputBindingDescription GetBindingDescription();//idk about param
        static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescriptions(const VertexAttributeDescriptionsInfo& info = {
            {VertexFormat::Vec2Float, 0},
            {VertexFormat::Vec3Float, 1}
        });

        glm::vec2 pos;
        glm::vec3 color;
    };
    class Mesh2D : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(Mesh2D);

        Mesh2D(const Vertices2D& vertices, const Indices& indices = {});

        void Reset() noexcept override;

        const Vertices2D& GetVertices() const noexcept;
        const Indices& GetIndices() const noexcept;
    private:
        Vertices2D m_Vertices;
        Indices m_Indices;
    };
}