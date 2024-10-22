module;
#include "../private/headers/Core/GObject/GClass.hpp"
#include "../Utils/Debug.hpp"
export module GuelderEngine.Core:Application;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import GuelderEngine.Events;
import GuelderEngine.Debug;
import GuelderEngine.Layers;
import GuelderEngine.Actors;
import GuelderEngine.Utils;
import GuelderEngine.UserInterface;
import :Window;

import <functional>;
import <memory>;
import <string_view>;

export namespace GuelderEngine
{
    //namespace Events { struct WindowCloseEvent; struct BaseEvent; class EventDispatcher; }
    //namespace Layers { class Layer; }
    namespace Utils { class ResourceManager; }
    using namespace Vulkan;
    using namespace UserInterface;
    class IApplication
    {
    public:
        virtual ~IApplication() = default;

        virtual void Run() = 0;
        virtual void OnEvent(Events::BaseEvent& event) = 0;

    protected:
        std::unique_ptr<Window> m_Window;
    };

    /// <summary>
    /// Guelder Engine Application
    /// </summary>
    class GEApplication : public IApplication
    {
    private:
        using RenderSystem3D = Vulkan::RenderSystem<3, 4>;
        using RenderSystem2D = Vulkan::RenderSystem<2, 2>;
    public:
        /// <param name="executablePath">path where .exe file stores, can be found in main</param>
        /// <param name="info">info about size of window, its name, etc</param>
        ///  <param name="shaderInfo3D">info about shader-related settings</param>
        ///  <param name="shaderInfo2D">info about shader-related settings</param>
        GEApplication(
            const std::string_view& executablePath,
            const Window::WindowData& info = {},
            const std::string_view& vertexShaderVarName3D = "vert3D",
            const std::string_view& fragmentShaderVarName3D = "frag3D",
            const Vulkan::VertexAttributeDescriptionsInfo& shaderInfo3D =
            {
                {Vulkan::VertexFormat::Vec3Float, 0},
                {Vulkan::VertexFormat::Vec3Float, 1 }
            },
            const std::string_view& vertexShaderVarName2D = "vert2D",
            const std::string_view& fragmentShaderVarName2D = "frag2D",
            const Vulkan::VertexAttributeDescriptionsInfo& shaderInfo2D =
            {
                {Vulkan::VertexFormat::Vec2Float, 0},
                {Vulkan::VertexFormat::Vec3Float, 1 }
            }
        );
        ~GEApplication() override;

        DELETE_COPYING_AND_MOVING(GEApplication);

        void Run() override;
        void OnEvent(Events::BaseEvent& event) override;

        void PushLayer(Layers::Layer* layer);
        void PushOverlay(Layers::Layer* overlay);
        
        int GetFrameRate() const noexcept;

        static double GetTime();

        void SetShaderInfo3D(const Vulkan::ShaderInfo& shaderInfo);
        void SetShaderInfo2D(const Vulkan::ShaderInfo& shaderInfo);
        void EnableUI(const bool& enable);

        const UniquePtr<World>& GetWorld();
        const UniquePtr<UserInterfaceManager>& GetUserInterfaceManager();
        bool GetEnableUI() const noexcept;

        static const Vulkan::VulkanManager& GetVulkanManager();
        //static const UniquePtr<Vulkan::MeshAllocator2D>& GetMeshAllocator2D();
        //static const UniquePtr<Vulkan::MeshAllocator3D>& GetMeshAllocator3D();

        UniquePtr<GameMode> gameMode;

        Events::EventDispatcher eventDispatcher;
        Utils::ResourceManager resourceManager;
    private:
        bool OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept;

        static const Vulkan::VulkanManager m_VulkanManager;
        UniquePtr<Vulkan::Renderer> m_Renderer;
        UniquePtr<RenderSystem3D> m_RenderSystem3D;
        UniquePtr<RenderSystem2D> m_RenderSystem2D;
        UniquePtr<World> m_World;
        UniquePtr<UserInterfaceManager> m_UIManager;

        Layers::LayerStack m_LayerStack;

        const Debug::Logger m_Logger;

        bool m_CloseWindow;
        bool m_EnableUI;
    };
}