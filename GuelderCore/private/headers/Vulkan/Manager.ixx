module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
export module GuelderEngine.Vulkan:Manager;

import :IVulkanObject;
import :DebugManager;
import :DeviceManager;
import :Mesh;
import :Pipeline;
import GuelderEngine.Core.Types;
//import :VulkanSurfaceManager;

import <string_view>;
import <vector>;
import <optional>;

export namespace GuelderEngine::Vulkan
{
    /*
    * Manager which responsible for all about initialization of Vulkan API
    */
    class VulkanManager : public IVulkanObject
    {
    public:
        VulkanManager() = default;
        VulkanManager(GLFWwindow* glfwWindow, const std::string_view& name = "Guelder Engine Editor");
        virtual ~VulkanManager();

        DECLARE_COPY_AND_MOVING(VulkanManager);

        void Reset() noexcept override;

        static bool AreExtensionsSupported(const std::vector<const char*>& extensions);
        //must be called before Render
        //void SetMesh(const Mesh2D& mesh);
        /**
         * \param width window width
         * \param height window height
         */
        //void Render(uint width, uint height, bool& wasWindowResized, const Vulkan::Buffers::VertexBuffer& vertexBuffer,
        //const Vulkan::Buffers::IndexBuffer& indexBuffer, const SimplePushConstantData& push = {});

        template<uint dimension>
        Buffers::VertexBuffer<dimension> MakeVertexBuffer(const Mesh<dimension>& mesh) const
        {
            return m_DeviceManager.MakeVertexBuffer<dimension>(mesh.GetVertices());
        }
        template<uint dimension>
        Buffers::IndexBuffer MakeIndexBuffer(const Mesh<dimension>& mesh) const
        {
            return m_DeviceManager.MakeIndexBuffer(mesh.GetIndices());
        }

        const DeviceManager& GetDevice() const noexcept;
        void WaitDevice() const;
        const vk::Instance& GetInstance() const;
    private:
        static vk::Instance CreateVkInstance(const char* name);
        void Cleanup() const noexcept;

        vk::Instance m_Instance;

        DeviceManager m_DeviceManager;
#ifdef GE_DEBUG_VULKAN
        DebugManager m_DebugManager;
#endif //GE_DEBUG_VULKAN
    };
}