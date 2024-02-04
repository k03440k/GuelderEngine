module;
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
module GuelderEngine.Vulkan;
import :Renderer;

import :Swapchain;
import :DeviceManager;
import :QueueFamilyIndices;
import :Surface;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    Renderer::Renderer(GLFWwindow* window, const vk::Instance& instance, const DeviceManager& deviceManager, uint width, uint height)
        : Renderer(
            window,
            instance,
            deviceManager.GetDevice(), 
            deviceManager.GetPhysicalDevice(), 
            {width, height}, 
            deviceManager.GetCommandPool().GetCommandPool(), 
            deviceManager.GetQueueIndices()
        ) {}
    Renderer::Renderer
    (
        GLFWwindow* window,
        const vk::Instance& instance,
        const vk::Device& device, 
        const vk::PhysicalDevice& physicalDevice,
        const vk::Extent2D& extent,
        const vk::CommandPool& commandPool,
        const QueueFamilyIndices& queueFamilyIndices
    )
        : m_Surface(window, instance, physicalDevice), m_CurrentImageIndex(0), m_IsFrameStarted(false)
    {
        m_Swapchain = Swapchain(device, physicalDevice, m_Surface.GetSurface(), m_Surface.GetCapabilities(), m_Surface.GetFormat(), m_Surface.GetPresentMode(), extent, commandPool, queueFamilyIndices);
    }

    Renderer::Renderer(Renderer&& other) noexcept
    {
        m_Surface = other.m_Surface;
        m_Swapchain = std::forward<Swapchain>(other.m_Swapchain);
        m_IsFrameStarted = other.m_IsFrameStarted;
        m_CurrentImageIndex = other.m_CurrentImageIndex;

        other.Reset();
    }
    Renderer& Renderer::operator=(Renderer&& other) noexcept
    {
        m_Surface = other.m_Surface;
        m_Swapchain = std::forward<Swapchain>(other.m_Swapchain);
        m_IsFrameStarted = other.m_IsFrameStarted;
        m_CurrentImageIndex = other.m_CurrentImageIndex;

        other.Reset();

        return *this;
    }
    void Renderer::Reset() noexcept
    {
        m_Surface.Reset();
        m_Swapchain.Reset();
        m_CurrentImageIndex = 0;
        m_IsFrameStarted = false;
    }
    void Renderer::Cleanup(const vk::Device& device,const vk::Instance& instance, const vk::CommandPool& commandPool) const noexcept
    {
        m_Swapchain.Cleanup(device, commandPool);
        m_Surface.Cleanup(instance);
    }
}
namespace GuelderEngine::Vulkan
{
    vk::CommandBuffer Renderer::BeginFrame(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::CommandPool& commandPool, 
        const vk::Extent2D& extent, 
        const QueueFamilyIndices& queueFamilyIndices)
    {
        GE_CLASS_ASSERT(!m_IsFrameStarted, "Cannot start rendering while frame is already in progress");

        m_Swapchain.GetCurrentFrame().WaitForImage(device);
        
        try
        {
            m_CurrentImageIndex = device.acquireNextImageKHR(m_Swapchain.GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_Swapchain.GetCurrentFrame().sync.GetImageAvailableSemaphore(), nullptr).value;
        }
        catch(const vk::OutOfDateKHRError&)
        {
            m_Swapchain.Recreate(device, physicalDevice, m_Surface.GetSurface(), m_Surface.GetCapabilities(), m_Surface.GetFormat(), m_Surface.GetPresentMode(), extent, commandPool, queueFamilyIndices);
            return nullptr;
        }

        m_IsFrameStarted = true;

        const auto& currentCommandBuffer = m_Swapchain.GetCurrentFrame().commandBuffer;
        constexpr vk::CommandBufferBeginInfo commandBufferBeginInfo{};
        currentCommandBuffer.begin(commandBufferBeginInfo);

        return currentCommandBuffer;
    }
    void Renderer::EndFrame(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::CommandPool& commandPool,
        const vk::Queue& graphicsQueue,
        const vk::Queue& presentQueue,
        const vk::Extent2D& extent,
        bool& wasWindowResized,
        const QueueFamilyIndices& queueFamilyIndices
    )
    {
        GE_CLASS_ASSERT(m_IsFrameStarted, "Cannot end frame when frame was not started");

        const auto& currentFrame = m_Swapchain.GetCurrentFrame();
        const auto& currentCommandBuffer = currentFrame.commandBuffer;

        currentCommandBuffer.end();

        const vk::Semaphore waitSemaphores[] = { currentFrame.sync.GetImageAvailableSemaphore() };
        const vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
        const vk::Semaphore signalSemaphores[] = { currentFrame.sync.GetImageRenderFinishedSemaphore() };

        const vk::SubmitInfo submitInfo(
            1,
            waitSemaphores,
            waitStages,
            1,
            &currentCommandBuffer,
            1,
            signalSemaphores
        );
        currentFrame.ResetFence(device);

        graphicsQueue.submit(submitInfo, currentFrame.sync.GetFlightFence());

        const vk::SwapchainKHR swapchains[] = { m_Swapchain.GetSwapchain() };

        const vk::PresentInfoKHR presentInfo(
            1,
            signalSemaphores,
            1,
            swapchains,
            &m_CurrentImageIndex
        );

        vk::Result presentResult;
        try
        {
            presentResult = presentQueue.presentKHR(presentInfo);
        }
        catch(const vk::OutOfDateKHRError&)
        {
            presentResult = vk::Result::eErrorOutOfDateKHR;
        }

        if(presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || wasWindowResized)
        {
            device.waitIdle();
            m_Swapchain.Recreate(device, physicalDevice, m_Surface.GetSurface(), m_Surface.GetCapabilities(), m_Surface.GetFormat(), m_Surface.GetPresentMode(), extent, commandPool, queueFamilyIndices);
            wasWindowResized = false;
        }

        m_IsFrameStarted = false;

        m_Swapchain.GetCurrentFrameNumber() = (m_Swapchain.GetCurrentFrameNumber() + 1) % m_Swapchain.GetMaxFramesInFlight();
    }
    void Renderer::BeginSwapchainRenderPass(const vk::CommandBuffer& commandBuffer) const
    {
        GE_CLASS_ASSERT(m_IsFrameStarted, "Cannot start render pass when frame was not started");
        GE_CLASS_ASSERT(commandBuffer == m_Swapchain.GetCurrentFrame().commandBuffer, "cannot begin render pass, when input buffer is different from the current one");

        const float blueValue = /*(sin(glfwGetTime()) / 2.0f) + 0.5f*/.25f;
        std::array clearValues
        {
            vk::ClearValue{ { 0.25f, 0.25f, blueValue, 1.0f } },
            vk::ClearValue{ {1.0f, 0} }
        };

        const vk::RenderPassBeginInfo renderPassBeginInfo(
            m_Swapchain.GetRenderPass(),
            m_Swapchain.GetFrames()[m_CurrentImageIndex].framebuffer,
            vk::Rect2D({ 0, 0 }, m_Swapchain.GetExtent2D()),
            clearValues.size(),
            clearValues.data()
        );

        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        const vk::Viewport viewport{
            0.0f,
                0.0f,
                static_cast<float>(m_Swapchain.GetExtent2D().width),
                static_cast<float>(m_Swapchain.GetExtent2D().height),
                0.f,
                1.0f
        };
        const vk::Rect2D scissors{{0, 0}, m_Swapchain.GetExtent2D()};

        commandBuffer.setViewport(0, 1, &viewport);
        commandBuffer.setScissor(0, 1, &scissors);
    }
    void Renderer::EndSwapchainRenderPass(const vk::CommandBuffer& commandBuffer) const
    {
        GE_CLASS_ASSERT(m_IsFrameStarted, "Cannot start end pass when frame was not started");
        GE_CLASS_ASSERT(commandBuffer == m_Swapchain.GetCurrentFrame().commandBuffer, "cannot end render pass, when input buffer is different from the current one");

        commandBuffer.endRenderPass();
    }
    const Swapchain& Renderer::GetSwapchain() const
    {
        return m_Swapchain;
    }
    const Surface& Renderer::GetSurface() const
    {
        return m_Surface;
    }
    bool Renderer::IsFrameInProgress() const noexcept
    {
        return m_IsFrameStarted;
    }
}