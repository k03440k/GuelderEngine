module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include <compare>
#include <glfw/glfw3.h>
module GuelderEngine.Core;
import :Application;

import GuelderEngine.Layers;
import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import GuelderEngine.Game;
import :ResourceManager;
import :Window;

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

        m_VulkanManager = std::make_unique<Vulkan::VulkanManager>(m_Window->GetGLFWWindow(), info.title);
        m_Renderer = std::make_unique<Vulkan::Renderer>(
            m_VulkanManager->GetDevice(),
            info.width, info.height
        );
        m_RenderSystem = std::make_unique<Vulkan::RenderSystem>(
            m_VulkanManager->GetDevice().GetDevice(), 
            m_Renderer->GetSwapchain().GetRenderPass(), 
            Vulkan::ShaderInfo
            {
                resourceManager.GetFullPathToRelativeFileViaVar(vertexShaderVarName),
                resourceManager.GetFullPathToRelativeFileViaVar(fragmentShaderVarName),
                shaderInfo
            }
            );
        m_World = std::make_unique<World>();
    }
    GEApplication::~GEApplication()
    {
        m_VulkanManager->WaitDevice();
        m_RenderSystem->Cleanup(m_VulkanManager->GetDevice().GetDevice());
        m_Renderer->Cleanup(m_VulkanManager->GetDevice());
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
            //temp
            const Vulkan::SimplePushConstantData data(
                static_cast<Object2D*>(m_World->GetRenderActor().get())->transform.Mat2(),
                { sin(glfwGetTime()) / 5.0f * cos(glfwGetTime()) , sin(glfwGetTime()) / 8.0f * tan(glfwGetTime()) },//idk how does it work
                { (cos(glfwGetTime()) / 5.0f) + 0.5f, (sin(glfwGetTime()) / 2.0f) + 0.5f, (sin(glfwGetTime()) / 2.0f) + 0.5f }
            );

            if(m_Window->GetData().width != 0 && m_Window->GetData().height != 0)
                if(const auto& commandBuffer = m_Renderer->BeginFrame(m_VulkanManager->GetDevice(), { m_Window->GetData().width, m_Window->GetData().height}))
                {
                    m_Renderer->BeginSwapchainRenderPass(commandBuffer);
                    //insert render here
                    m_RenderSystem->Render(commandBuffer, data, m_World->GetRenderActor()->vertexBuffer, m_World->GetRenderActor()->indexBuffer);
                    m_Renderer->EndSwapchainRenderPass(commandBuffer);
                    m_Renderer->EndFrame(m_VulkanManager->GetDevice(), { m_Window->GetData().width, m_Window->GetData().height }, m_Window->WasWindowResized());
                }
            //    m_VulkanManager->Render(m_Window->GetData().width, m_Window->GetData().height, m_Window->WasWindowResized(), m_World->GetRenderActor()->vertexBuffer, m_World->GetRenderActor()->indexBuffer, data);//temp

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
    double GEApplication::GetTime()
    {
        return glfwGetTime();
    }
    bool GEApplication::OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept
    {
        m_CloseWindow = true;
        return m_CloseWindow;
    }
    void GEApplication::SetShaderInfo(const Vulkan::ShaderInfo& shaderInfo)
    {
        m_RenderSystem->SetShaderInfo(m_VulkanManager->GetDevice().GetDevice(), m_Renderer->GetSwapchain().GetRenderPass(), shaderInfo);
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
    void GEApplication::SpawnRenderActor(SharedPtr<Object2D> renderActor)
    {
        renderActor->vertexBuffer = m_VulkanManager->MakeVertexBuffer(renderActor->GetMesh());
        renderActor->indexBuffer = m_VulkanManager->MakeIndexBuffer(renderActor->GetMesh());
        m_World->SpawnRenderActor(renderActor);
    }
#pragma endregion
}