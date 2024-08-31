module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include <compare>
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
module GuelderEngine.Core;
import :Application;

import GuelderEngine.Layers;
import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;
import GuelderEngine.Actors;
import GuelderEngine.Utils;
import :Window;

import <string>;
import <string_view>;
import <vector>;
import <memory>;
import <functional>;
import <thread>;
import <ranges>;
import <chrono>;

namespace GuelderEngine
{
    //UniquePtr<Vulkan::MeshAllocator2D> GEApplication::m_MeshAllocator2D = std::make_unique<Vulkan::MeshAllocator2D>();
    //UniquePtr<Vulkan::MeshAllocator3D> GEApplication::m_MeshAllocator3D = std::make_unique<Vulkan::MeshAllocator3D>();
    //std::unique_ptr<Vulkan::VulkanManager> GEApplication::m_VulkanManager = std::make_unique<Vulkan::VulkanManager>("Guelder Engine Editor");
#pragma region GEApplication
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
    GEApplication::GEApplication(
        const std::string_view& executablePath,
        const Window::WindowData& info,
        const std::string_view& vertexShaderVarName3D,
        const std::string_view& fragmentShaderVarName3D,
        const Vulkan::VertexAttributeDescriptionsInfo& shaderInfo3D,
        const std::string_view& vertexShaderVarName2D,
        const std::string_view& fragmentShaderVarName2D,
        const Vulkan::VertexAttributeDescriptionsInfo& shaderInfo2D
    )
        : resourceManager(executablePath)
    {
        m_Window = std::make_unique<Window>(Window::WindowData(info.width, info.height, info.title.data()));
        m_Window->SetCallback(BIND_EVENT_FUNC(GEApplication::OnEvent));

        //m_VulkanManager = std::make_unique<Vulkan::VulkanManager>(info.title);
        m_Renderer = std::make_unique<Vulkan::Renderer>(
            m_Window->GetGLFWWindow(),
            m_VulkanManager.GetInstance(),
            m_VulkanManager.GetDevice(),
            info.width, info.height
        );
        m_RenderSystem3D = std::make_unique<RenderSystem3D>
            (
                m_VulkanManager.GetDevice().GetDevice(),
                m_Renderer->GetSwapchain().GetRenderPass(),
                Vulkan::ShaderInfo
                {
                    resourceManager.GetFullPathToRelativeFileViaVar(vertexShaderVarName3D),
                    resourceManager.GetFullPathToRelativeFileViaVar(fragmentShaderVarName3D),
                    shaderInfo3D
                }
        );
        m_RenderSystem2D = std::make_unique<RenderSystem2D>
            (
                m_VulkanManager.GetDevice().GetDevice(),
                m_Renderer->GetSwapchain().GetRenderPass(),
                Vulkan::ShaderInfo
                {
                    resourceManager.GetFullPathToRelativeFileViaVar(vertexShaderVarName2D),
                    resourceManager.GetFullPathToRelativeFileViaVar(fragmentShaderVarName2D),
                    shaderInfo2D
                }
        );
        gameMode = std::make_unique<GameMode>();
        m_World = std::make_unique<World>();
    }
    GEApplication::~GEApplication()
    {
        m_VulkanManager.GetDevice().WaitIdle();
        m_RenderSystem3D->Cleanup(m_VulkanManager.GetDevice().GetDevice());
        m_RenderSystem2D->Cleanup(m_VulkanManager.GetDevice().GetDevice());
        m_Renderer->Cleanup(m_VulkanManager.GetDevice(), m_VulkanManager.GetInstance());
        m_World->CleanupRenderActors(m_VulkanManager.GetDevice().GetDevice());
        MeshComponent<2>::GetMeshAllocator().Cleanup(m_VulkanManager.GetDevice().GetDevice());
        MeshComponent<3>::GetMeshAllocator().Cleanup(m_VulkanManager.GetDevice().GetDevice());
        //m_VulkanManager.reset();
    }
    void GEApplication::Run()
    {
        gameMode->GetPlayerController()->SetupInput();
        gameMode->BeginPlay();
        m_World->BeginPlay();

        //TODO: come up with role of camera and its position
        //TODO: come up with transforming of object rendering and color
        //TODO: make PlayerController which will be responsible for camera
        //CameraActor3D tempCameraActor{};
        //auto& camera = tempCameraActor.camera;

        auto currentTime = std::chrono::high_resolution_clock::now();

        while(!m_CloseWindow)
        {
            m_Window->OnUpdate();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            m_World->UpdateActors(frameTime);

            if(m_Window->GetData().width != 0 && m_Window->GetData().height != 0)
            {
                gameMode->GetPlayerController()->Update(m_Window->GetGLFWWindow(), frameTime);

                if(const auto& commandBuffer = m_Renderer->BeginFrame(m_VulkanManager.GetDevice(), { m_Window->GetData().width, m_Window->GetData().height }))
                {
                    auto ratio = m_Renderer->GetSwapchain().GetAspectRatio();
                    //camera->SetOrthographicProjection(-ratio, ratio, -1, 1, -1, 1);
                    //camera.SetPerspectiveProjection(glm::radians(50.f), ratio, 0.1f, 10.f);

                    CameraComponent* cameraComponent = gameMode->GetPlayerController()->camera;

                    auto projectionView1 = cameraComponent->GetProjection() * cameraComponent->GetViewMatrix();

                    cameraComponent->SetViewYXZ(cameraComponent->transform.translation, cameraComponent->transform.rotation);

                    cameraComponent->SetPerspectiveProjection(glm::radians(50.f), ratio, 0.1f, 100.0f);

                    auto projectionView = cameraComponent->GetProjection() * cameraComponent->GetViewMatrix();

                    m_Renderer->BeginSwapchainRenderPass(commandBuffer);

                    std::ranges::for_each(m_World->GetActors3D(), [this, &commandBuffer, &projectionView](const SharedPtr<Actor3D>& actor)
                        {
                            if(actor->IsComplete())
                                m_RenderSystem3D->Render
                                (
                                    commandBuffer,
                                    MeshComponent3D::GetVertexBuffer(),
                                    MeshComponent3D::GetIndexBuffer(),
                                    actor->meshComponent->GetVertexSector() ? actor->meshComponent->GetVertexSector()->GetSize() : 0,
                                    actor->meshComponent->GetIndexSector() ? actor->meshComponent->GetIndexSector()->GetSize() : 0,
                                    actor->meshComponent->GetIndexSector() ? actor->meshComponent->GetVertexSector()->starts : 0,
                                    actor->meshComponent->GetIndexSector() ? actor->meshComponent->GetIndexSector()->starts : 0,
                                    MatFromRenderActorTransform<3, 4>(actor->transform),
                                    projectionView
                                );
                        });
                    std::ranges::for_each(m_World->GetActors2D(), [this, &commandBuffer](const SharedPtr<Actor2D>& actor)
                        {
                            m_RenderSystem2D->Render
                            (
                                commandBuffer,
                                MeshComponent2D::GetVertexBuffer(),
                                MeshComponent2D::GetIndexBuffer(),
                                actor->meshComponent->GetVertexSector() ? actor->meshComponent->GetVertexSector()->GetSize() : 0,
                                actor->meshComponent->GetIndexSector() ? actor->meshComponent->GetIndexSector()->GetSize() : 0,
                                actor->meshComponent->GetIndexSector() ? actor->meshComponent->GetVertexSector()->starts : 0,
                                actor->meshComponent->GetIndexSector() ? actor->meshComponent->GetIndexSector()->starts : 0,
                                MatFromRenderActorTransform<2, 2>(actor->transform),
                                actor->transform.position
                            );
                        });

                    m_Renderer->EndSwapchainRenderPass(commandBuffer);
                    m_Renderer->EndFrame(m_VulkanManager.GetDevice(), { m_Window->GetData().width, m_Window->GetData().height }, m_Window->WasWindowResized());
                }
            }

            //for(Layers::Layer* layer : m_LayerStack)
            //    layer->OnUpdate();
        }
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
    int GEApplication::GetFrameRate() const noexcept
    {
        return m_Window->GetData().GetFrameRate();
    }
    double GEApplication::GetTime()
    {
        return glfwGetTime();
    }
    bool GEApplication::OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept
    {
        m_CloseWindow = true;
        return m_CloseWindow;
    }
    void GEApplication::SetShaderInfo3D(const Vulkan::ShaderInfo& shaderInfo)
    {
        m_RenderSystem3D->SetShaderInfo(m_VulkanManager.GetDevice().GetDevice(), m_Renderer->GetSwapchain().GetRenderPass(), shaderInfo);
    }
    void GEApplication::SetShaderInfo2D(const Vulkan::ShaderInfo& shaderInfo)
    {
        m_RenderSystem2D->SetShaderInfo(m_VulkanManager.GetDevice().GetDevice(), m_Renderer->GetSwapchain().GetRenderPass(), shaderInfo);
    }
    const UniquePtr<World>& GEApplication::GetWorld()
    {
        return m_World;
    }
    const Vulkan::VulkanManager& GEApplication::GetVulkanManager()
    {
        return m_VulkanManager;
    }
#pragma endregion
}