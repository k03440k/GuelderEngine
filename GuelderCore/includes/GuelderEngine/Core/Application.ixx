module;
#include "../src/headers/Core/GObject/GClass.hpp"
//#include "Utils/Utils.hpp"
//
//#include "Events/Event.hpp"
//#include "Events/ApplicationEvent.hpp"
//
//#include "Layers/LayerStack.hpp"
//
//#include "../src/headers/Vulkan/VulkanManager.hpp"
//
//#include <string_view>
//#include <functional>
//#include <memory>
export module GuelderEngine.Core:Application;

import GuelderEngine.Vulkan;
import GuelderEngine.Events;
import GuelderEngine.Debug;
import GuelderEngine.Layers;

import <functional>;
import <memory>;
import <string_view>;

export namespace GuelderEngine
{
    //idk is it normal
    //namespace Events { struct WindowCloseEvent; struct Event; class EventDispatcher; }
    namespace Layers { class Layer; }

    class IApplication
    {
    public:
        virtual ~IApplication() = default;

        virtual void Run() = 0;
        virtual void OnEvent(Events::Event& event) = 0;

    protected:
        std::unique_ptr<class Window> m_Window;
    };

    //Guelder Engine Application
    class GEApplication : public IApplication, public Vulkan::VulkanManager, INHERIT_GClass(GEApplication)
    {
    public:
        GEApplication(const Types::ushort& windowWidth = 640, const Types::ushort& windowHeight = 480,
                    const std::string_view& windowTitle = "Guelder Engine Window", const std::function<void()>& callOnUpdate = [] {});
        virtual ~GEApplication();

        GEApplication(const GEApplication&) = delete;
        GEApplication(GEApplication&&) = delete;
        GEApplication& operator=(const GEApplication&) = delete;
        GEApplication& operator=(GEApplication&&) = delete;

        void Run() override;
        void Run(const std::function<void()>& callOnUpdate);
        void OnEvent(Events::Event& event) override;

        void PushLayer(Layers::Layer* layer);
        void PushOverlay(Layers::Layer* overlay);

        //Window& GetWindow() { return *window; }

        void SetOnUpdateFunc(const std::function<void()>& onUpdate) noexcept;
        const std::function<void()>& GetOnUpdateFunc();

        Events::EventDispatcher eventDispatcher;
    private:
        bool OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept;

        //std::function<void()> m_CallOnUpdate;

        //std::unique_ptr<class Window> m_Window;

        Layers::LayerStack m_LayerStack;

        const Debug::Logger m_Logger;

        bool m_CloseWindow = false;
    };
}