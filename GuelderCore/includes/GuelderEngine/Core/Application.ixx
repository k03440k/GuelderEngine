module;
#include "../src/headers/Core/GObject/GClass.hpp"
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
        /*
         *@param executablePath - path where .exe file stores, can be found in main
         *@param info - info about size of window, its name, etc
         *@param vertexShaderVarName - name of custom vertex shader
         *@param fragmentShaderVarName - name of custom fragment shader
        */
        GEApplication(
            const std::string_view& executablePath, 
            const Window::WindowData& info = {}, 
            const std::string_view& vertexShaderVarName = "vert",
            const std::string_view& fragmentShaderVarName = "frag"
        );
        virtual ~GEApplication();

        DELETE_COPY_AND_MOVING(GEApplication);

        void Run() override;
        void Run(const std::function<void()>& callOnUpdate);
        void OnEvent(Events::BaseEvent& event) override;

        void PushLayer(Layers::Layer* layer);
        void PushOverlay(Layers::Layer* overlay);

        //Window& GetWindow() { return *window; }

        void SetOnUpdateFunc(const std::function<void()>& onUpdate) noexcept;
        const std::function<void()>& GetOnUpdateFunc();

        Events::EventDispatcher eventDispatcher;
        Utils::ResourceManager resourceManager;
    private:
        std::unique_ptr<Vulkan::VulkanManager> m_VulkanManager;//TODO: remove std::unique_ptr and define for all Vulkan classes moving ctors and operators=

        bool OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept;

        //std::function<void()> m_CallOnUpdate;

        //std::unique_ptr<class Window> m_Window;

        Layers::LayerStack m_LayerStack;

        const Debug::Logger m_Logger;

        bool m_CloseWindow = false;
    };
}