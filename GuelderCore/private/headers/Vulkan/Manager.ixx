module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:Manager;

import :IVulkanObject;
import :DebugManager;
import :DeviceManager;
import :Mesh;
import :Pipeline;
import GuelderEngine.Core.Types;

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
        VulkanManager(const std::string_view& name = "Guelder Engine Editor");
        virtual ~VulkanManager();

        DECLARE_MOVING(VulkanManager);
        DECLARE_DEFAULT_COPYING(VulkanManager);

        void Reset() noexcept override;

        static bool AreExtensionsSupported(const std::vector<const char*>& extensions);

        template<uint dimension>
        Buffers::VertexBuffer CreateVertexBuffer(const Mesh<dimension>& mesh) const
        {
            return m_DeviceManager.CreateVertexBuffer<dimension>(mesh.GetVertices());
        }
        template<uint dimension>
        Buffers::IndexBuffer CreateIndexBuffer(const Mesh<dimension>& mesh) const
        {
            return m_DeviceManager.CreateIndexBuffer(mesh.GetIndices());
        }

        static const VulkanManager& Get();

        const DeviceManager& GetDeviceManager() const;
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