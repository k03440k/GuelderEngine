module;
#include "../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Core:Application;

import GuelderEngine.Vulkan;
import GuelderEngine.Events;
import GuelderEngine.Debug;
import GuelderEngine.Layers;
import :ResourceManager;
import :Window;

import <functional>;
import <memory>;
import <string_view>;

export namespace GuelderEngine
{
    namespace Vulkan
    {
        class VulkanManager;
    }

    //namespace Events { struct WindowCloseEvent; struct BaseEvent; class EventDispatcher; }
    namespace Layers { class Layer; }

    class IApplication
    {
    public:
        virtual ~IApplication() = default;

        virtual void Run() = 0;
        virtual void OnEvent(Events::BaseEvent& event) = 0;

    protected:
        std::unique_ptr<Window> m_Window;
    };

    //Guelder Engine Application
    class GEApplication : public IApplication, INHERIT_GClass(GEApplication)
    {
    public:
        using UpdateFunc = std::function<void()>;
        /**
         *@param executablePath - path where .exe file stores, can be found in main
         *@param info - info about size of window, its name, etc
         *@param shaderInfo - info about shader-related settings
        */
        GEApplication(
            const std::string_view& executablePath,
            const Window::WindowData& info = {},
            const std::string_view& vertexShaderVarName = "vert",
            const std::string_view& fragmentShaderVarName = "frag",
            const Vulkan::VertexAttributeDescriptionsInfo& shaderInfo =
            {
                {Vulkan::VertexFormat::Vec2Float, 0},
                { Vulkan::VertexFormat::Vec3Float, 1 }
            }
        );
        virtual ~GEApplication() = default;

        DELETE_COPY_AND_MOVING(GEApplication);

        void Run() override;
        void Run(const std::function<void()>& callOnUpdate);
        void OnEvent(Events::BaseEvent& event) override;

        void PushLayer(Layers::Layer* layer);
        void PushOverlay(Layers::Layer* overlay);

        void SetOnUpdateFunc(const UpdateFunc& onUpdate) noexcept;
        int GetFrameRate() const noexcept { return m_Window->GetData().GetFrameRate(); }

        void SetMesh(const Vulkan::Mesh& mesh);
        void SetShaderInfo(const Vulkan::ShaderInfo& shaderInfo);

        Events::EventDispatcher eventDispatcher;
        Utils::ResourceManager resourceManager;
    private:
        std::unique_ptr<Vulkan::VulkanManager> m_VulkanManager;

        bool OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept;
        
        UpdateFunc m_OnUpdate;

        Layers::LayerStack m_LayerStack;

        const Debug::Logger m_Logger;

        bool m_CloseWindow = false;
    };
}