#include "../includes/GuelderEngine/Application.hpp"

#include "../includes/GuelderEngine/Utils/Debug.hpp"

#include "../headers/Window.hpp"

#include "../../includes/GuelderEngine/Layers/Layer.hpp"

//#include <stdexcept>
#include <string>//<-- added this
//#include <iostream>

namespace GuelderEngine
{
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
    Application::Application(const Utils::ushort& windowWidth, const Utils::ushort& windowHeight,
        const std::string_view& windowTitle, const std::function<void()>& callOnUpdate)
    {
        m_Window = std::make_unique<Window>(Window::WindowData(windowTitle.data(), windowWidth, windowHeight));
        m_Window->SetCallback(BIND_EVENT_FUNC(Application::OnEvent));
        //this->m_CallOnUpdate = callOnUpdate;
    }
    Application::~Application()
    {
        m_Window->~Window();
    }
    void Application::Start()
    {
        while (!m_CloseWindow)
        {
            m_Window->OnUpdate();

            if (!m_LayerStack.IsEmpty())
            {
                for (Layers::Layer* layer : m_LayerStack)
                    layer->OnUpdate();
            }
        }
    }
    void Application::Start(const std::function<void()>& callOnUpdate)
    {
        while (!m_CloseWindow)
        {
            m_Window->OnUpdate(callOnUpdate);

            if (!m_LayerStack.IsEmpty())
            {
                for (Layers::Layer* layer : m_LayerStack)
                    layer->OnUpdate();
            }
        }
        m_Window->onUpdate = callOnUpdate;
        //this->m_CallOnUpdate = callOnUpdate;
    }
    void Application::OnEvent(Events::Event& event)
    {
        Events::EventDispatcher dispatcher;
        dispatcher.AddEventListener<Events::WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowCloseEvent));
        dispatcher.Dispatch(event);

        //Utils::Log::Message(event.ToString());

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.isHandled)
                break;
        }
    }
    void Application::PushLayer(Layers::Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }
    void Application::PushOverlay(Layers::Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }
    void Application::SetOnUpdateFunc(const std::function<void()>& onUpdate) noexcept { m_Window->onUpdate = onUpdate; }
    const std::function<void()>& Application::GetOnUpdateFunc() { return m_Window->onUpdate; }
    bool Application::OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept
    {
        m_CloseWindow = true;
        return m_CloseWindow;
    }
}