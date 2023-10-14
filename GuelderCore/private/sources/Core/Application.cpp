module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include <compare>
#include <glfw/glfw3.h>
module GuelderEngine.Core;
import :Application;

import :Window;
import GuelderEngine.Layers;
import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import :ResourceManager;
import GuelderEngine.Game;

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
        : resourceManager(executablePath), m_OnUpdate([] {})
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
        m_World = std::make_unique<World>();
    }
    GEApplication::~GEApplication()
    {
        m_VulkanManager->WaitDevice();
        for(auto& actor : m_World->GetRenderActors())
            actor->Cleanup(m_VulkanManager->GetDevice().GetDevice());
        //m_VulkanManager.reset();
    }
    void GEApplication::Run()
    {
        m_World->Begin();

        while(!m_CloseWindow)
        {
            m_Window->OnUpdate();

            const SimplePushConstantData data(
                m_World->GetRenderActor()->transform.Mat2(),
                { sin(glfwGetTime()) / 5.0f * cos(glfwGetTime()) , sin(glfwGetTime()) / 8.0f * tan(glfwGetTime()) },//idk how does it work
                { (cos(glfwGetTime()) / 5.0f) + 0.5f, (sin(glfwGetTime()) / 2.0f) + 0.5f, (sin(glfwGetTime()) / 2.0f) + 0.5f });

            if(m_Window->GetData().width != 0 && m_Window->GetData().height != 0)
                m_VulkanManager->Render(m_Window->GetData().width, m_Window->GetData().height, m_Window->WasWindowResized(), m_World->GetRenderActor()->vertexBuffer, m_World->GetRenderActor()->indexBuffer, data);//temp

            m_OnUpdate();

            m_World->UpdateActors();

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

        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if(event.isHandled)
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
    void GEApplication::SetShaderInfo(const Vulkan::ShaderInfo& shaderInfo)
    {
        m_VulkanManager->SetShaderInfo(shaderInfo);
    }
    World& GEApplication::GetWorld()
    {
        return *m_World;
    }
    void GEApplication::SpawnRenderActor(const Object2DCreateInfo& renderActor)
    {
        Object2D obj{ m_VulkanManager->MakeVertexBuffer(renderActor.mesh), m_VulkanManager->MakeIndexBuffer(renderActor.mesh) };
        obj.SetMesh(renderActor.mesh);
        obj.transform = renderActor.transform;
        m_World->SpawnRenderActor(MakeActor(std::move(obj)));
    }
    void GEApplication::SpawnRenderActor(Object2DPtr renderActor)
    {
        renderActor->vertexBuffer = m_VulkanManager->MakeVertexBuffer(renderActor->GetMesh());
        renderActor->indexBuffer = m_VulkanManager->MakeIndexBuffer(renderActor->GetMesh());
        m_World->SpawnRenderActor(renderActor);
    }
    /*void GEApplication::SetMesh(const Vulkan::Mesh2D& mesh)
    {
        m_VulkanManager->SetMesh(mesh);
    }*/
#pragma endregion
}