#pragma once

#include "Utils/Utils.hpp"

#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"

//#include "Layers/Layer.hpp"
#include "Layers/LayerStack.hpp"

#include <functional>
#include <memory>
//#include <string>
#include <string_view>//<-- added this

//#include "../src/headers/Window.hpp"

namespace GuelderEngine
{
    //idk is it normal
    //namespace Events { struct WindowCloseEvent; struct Event; class EventDispatcher; }
    namespace Layers { class Layer; class LayerStack; }

    class Application
    {
    public:
        Application(const Utils::ushort& windowWidth, const Utils::ushort& windowHeight,
                    const std::string_view& windowTitle, const std::function<void()>& callOnUpdate = []{});
        virtual ~Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        virtual void Start();
        void Start(const std::function<void()>& callOnUpdate);
        virtual void OnEvent(Events::Event& event);

        void PushLayer(Layers::Layer* layer);
        void PushOverlay(Layers::Layer* overlay);

        //Window& GetWindow() { return *window; }

        void SetOnUpdateFunc(const std::function<void()>& onUpdate) noexcept;
        const std::function<void()>& GetOnUpdateFunc();

        Events::EventDispatcher eventDispatcher;
    private:
        bool OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept;

        //std::function<void()> m_CallOnUpdate;

        std::unique_ptr<class Window> m_Window;

        Layers::LayerStack m_LayerStack;

        bool m_CloseWindow = false;
    };
}