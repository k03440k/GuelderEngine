module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
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
import <thread>;

namespace GuelderEngine
{
#pragma region GEApplication
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
    GEApplication::GEApplication(
        const std::string_view& executablePath, 
        const Window::WindowData& info,
        const std::string_view& vertexShaderVarName,
        const std::string_view& fragmentShaderVarName,
        const Vulkan::VertexAttributeDescriptionsInfo& shaderInfo
    )
    : resourceManager(executablePath), m_OnUpdate([]{})
    {
        m_Window = std::make_unique<Window>(Window::WindowData(info.width, info.height, info.title.data()));
        m_Window->SetCallback(BIND_EVENT_FUNC(GEApplication::OnEvent));

        m_VulkanManager = std::make_unique<Vulkan::VulkanManager>
        (
            m_Window->GetGLFWWindow(),
            m_Window->GetData().width,
            m_Window->GetData().height,
            Vulkan::ShaderInfo{
                resourceManager.GetFullPathToRelativeFileViaVar(vertexShaderVarName),
                resourceManager.GetFullPathToRelativeFileViaVar(fragmentShaderVarName),
                shaderInfo
            },
            info.title
        );
    }
    void GEApplication::Run()
    {
        while(!m_CloseWindow)
        {
            m_Window->OnUpdate();

            if(m_Window->GetData().width != 0 && m_Window->GetData().height != 0)
               m_VulkanManager->Render(m_Window->GetData().width, m_Window->GetData().height, m_Window->WasWindowResized());

            m_OnUpdate();

            if(!m_LayerStack.IsEmpty())
            {
                for(Layers::Layer* layer : m_LayerStack)
                    layer->OnUpdate();
            }
        }
    }
    void GEApplication::Run(const std::function<void()>& callOnUpdate)
    {
        SetOnUpdateFunc(callOnUpdate);
        Run();
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
    void GEApplication::SetOnUpdateFunc(const UpdateFunc& onUpdate) noexcept { m_OnUpdate = onUpdate; }
    bool GEApplication::OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept
    {
        m_CloseWindow = true;
        return m_CloseWindow;
    }
    void GEApplication::SetMesh(const Vulkan::Mesh& mesh)
    {
        m_VulkanManager->SetMesh(mesh);
    }
    void GEApplication::SetShaderInfo(const Vulkan::ShaderInfo& shaderInfo)
    {
        m_VulkanManager->SetShaderInfo(shaderInfo);
    }
#pragma endregion
}