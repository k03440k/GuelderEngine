module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Core;
import :Application;

import :Window;
import GuelderEngine.Layers;
import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :ResourceManager;

import <string>;
import <string_view>;
import <vector>;
import <memory>;
import <functional>;

namespace GuelderEngine
{
#pragma region GEApplication
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
    GEApplication::GEApplication(const std::string_view& executablePath, const Window::WindowData& info, const std::string_view& vertexShaderVarName,
        const std::string_view& fragmentShaderVarName)
    : resourceManager(executablePath)
    {
        m_Window = std::make_unique<Window>(Window::WindowData(info.title.data(), info.width, info.height));
        m_Window->SetCallback(BIND_EVENT_FUNC(GEApplication::OnEvent));

        m_VulkanManager = std::make_unique<Vulkan::VulkanManager>(
            m_Window->m_GLFWWindow, 
            resourceManager.GetFullPathToRelativeFileViaVar(vertexShaderVarName),
            resourceManager.GetFullPathToRelativeFileViaVar(fragmentShaderVarName),
            info.title);
    }
    GEApplication::~GEApplication()
    {
    }
    void GEApplication::Run()
    {
        while (!m_CloseWindow)
        {
            m_Window->OnUpdate();
            m_VulkanManager->Render();

            if (!m_LayerStack.IsEmpty())
            {
                for (Layers::Layer* layer : m_LayerStack)
                    layer->OnUpdate();
            }
        }
    }
    void GEApplication::Run(const std::function<void()>& callOnUpdate)
    {
        while (!m_CloseWindow)
        {
            m_Window->OnUpdate(callOnUpdate);
            m_VulkanManager->Render();

            if (!m_LayerStack.IsEmpty())
            {
                for (Layers::Layer* layer : m_LayerStack)
                    layer->OnUpdate();
            }
        }
        m_Window->onUpdate = callOnUpdate;
    }
    void GEApplication::OnEvent(Events::BaseEvent& event)
    {
        Events::EventDispatcher dispatcher;
        dispatcher.AddEventListener<Events::WindowCloseEvent>(BIND_EVENT_FUNC(GEApplication::OnWindowCloseEvent));
        dispatcher.Dispatch(event);

        //Utils::Log::Message(event.ToString());

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.isHandled)
                break;
        }
    }
    void GEApplication::PushLayer(Layers::Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }
    void GEApplication::PushOverlay(Layers::Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }
    void GEApplication::SetOnUpdateFunc(const std::function<void()>& onUpdate) noexcept { m_Window->onUpdate = onUpdate; }
    const std::function<void()>& GEApplication::GetOnUpdateFunc() { return m_Window->onUpdate; }
    bool GEApplication::OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept
    {
        m_CloseWindow = true;
        return m_CloseWindow;
    }
#pragma endregion
}