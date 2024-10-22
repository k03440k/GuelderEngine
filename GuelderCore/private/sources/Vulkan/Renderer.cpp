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
import :IVulkanObject;
import GuelderEngine.Core.Types;

import GuelderEngine.Layers;

namespace GuelderEngine::Vulkan
{
    Renderer::Renderer(GLFWwindow* window, const vk::Instance& instance, const DeviceManager& deviceManager, uint width, uint height)
        : Renderer(
            window,
            instance,
            deviceManager.GetDevice(),
            deviceManager.GetPhysicalDevice(),
            { width, height },
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
}
namespace GuelderEngine::Vulkan
{
    void Renderer::Reset() noexcept
    {
        m_Surface.Reset();
        m_Swapchain.Reset();
        m_CurrentImageIndex = 0;
        m_IsFrameStarted = false;
    }
    void Renderer::Cleanup(const vk::Device& device, const vk::Instance& instance, const vk::CommandPool& commandPool) const noexcept
    {
        m_Swapchain.Cleanup(device, commandPool);
        m_Surface.Cleanup(instance);
    }

    bool Renderer::BeginFrame(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::CommandPool& commandPool,
        const std::vector<vk::CommandBuffer>& commandBuffers,
        const vk::Extent2D& extent,
        const QueueFamilyIndices& queueFamilyIndices
    )
    {
        GE_ASSERT(!m_IsFrameStarted, "Cannot start rendering while frame is already in progress");

        m_Swapchain.GetCurrentFrame().WaitForFence(device);

        try
        {
            m_CurrentImageIndex = device.acquireNextImageKHR(m_Swapchain.GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_Swapchain.GetCurrentFrame().sync.GetImageAvailableSemaphore(), nullptr).value;
        }
        catch(const vk::OutOfDateKHRError&)
        {
            GE_LOG(VulkanCore, Warning, "BeginFrame RECREATE");
            m_Surface.Recreate(physicalDevice);
            m_Swapchain.Recreate(device, physicalDevice, m_Surface.GetSurface(), m_Surface.GetCapabilities(), m_Surface.GetFormat(), m_Surface.GetPresentMode(), extent, commandPool, queueFamilyIndices);
            return false;
        }

        m_IsFrameStarted = true;

        constexpr vk::CommandBufferBeginInfo commandBufferBeginInfo{};

        for(auto& commandBuffer : commandBuffers)
            commandBuffer.begin(commandBufferBeginInfo);

        return true;
    }
    bool Renderer::EndFrame(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::CommandPool& commandPool,
        const std::vector<vk::CommandBuffer>& commandBuffers,
        const vk::Queue& graphicsQueue,
        const vk::Queue& presentQueue,
        const vk::Extent2D& extent,
        bool& wasWindowResized,
        const QueueFamilyIndices& queueFamilyIndices
    )
    {
        GE_ASSERT(m_IsFrameStarted, "Cannot end frame when frame was not started");

        bool wasRecreated = false;

        const auto& currentFrame = m_Swapchain.GetCurrentFrame();

        for(auto& commandBuffer : commandBuffers)
            commandBuffer.end();

        const vk::Semaphore waitSemaphores[] = { currentFrame.sync.GetImageAvailableSemaphore() };
        const vk::Semaphore signalSemaphores[] = { currentFrame.sync.GetImageRenderFinishedSemaphore() };
        const vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

        const vk::SubmitInfo submitInfo(
            1,
            waitSemaphores,
            waitStages,
            (uint)commandBuffers.size(),
            commandBuffers.data(),
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
            wasRecreated = true;
            device.waitIdle();
            m_Surface.Recreate(physicalDevice);
            m_Swapchain.Recreate(device, physicalDevice, m_Surface.GetSurface(), m_Surface.GetCapabilities(), m_Surface.GetFormat(), m_Surface.GetPresentMode(), extent, commandPool, queueFamilyIndices);
            wasWindowResized = false;
        }

        m_IsFrameStarted = false;

        return wasRecreated;
    }
    void Renderer::BeginSwapchainRenderPass(const vk::CommandBuffer& commandBuffer) const
    {
        GE_ASSERT(m_IsFrameStarted, "Cannot start render pass when frame was not started");
        //GE_ASSERT(commandBuffer == m_Swapchain.GetCurrentFrame().commandBuffer, "cannot begin render pass, when input buffer is different from the current one");

        const float blueValue = /*(sin(glfwGetTime()) / 2.0f) + 0.5f*/.25f;
        std::array clearValues
        {
            vk::ClearValue{ { 0.25f, 0.25f, blueValue, 1.0f } },
            vk::ClearValue{ {1.0f, 0} }
        };

        const vk::RenderPassBeginInfo renderPassBeginInfo(
            m_Swapchain.GetRenderPass(),
            m_Swapchain.GetFrames()[m_CurrentImageIndex].frameBuffer,
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
        const vk::Rect2D scissors{ {0, 0}, m_Swapchain.GetExtent2D() };

        commandBuffer.setViewport(0, 1, &viewport);
        commandBuffer.setScissor(0, 1, &scissors);
    }
    void Renderer::EndSwapchainRenderPass(const vk::CommandBuffer& commandBuffer) const
    {
        GE_ASSERT(m_IsFrameStarted, "Cannot start end pass when frame was not started");
        GE_ASSERT(commandBuffer == m_Swapchain.GetCurrentFrame().commandBuffer, "cannot end render pass, when input buffer is different from the current one");

        commandBuffer.endRenderPass();
    }
    void Renderer::IncrementCurrentFrame()
    {
        m_Swapchain.currentFrameNumber = (m_Swapchain.currentFrameNumber + 1) % m_Swapchain.GetMaxFramesInFlight();
    }
}
namespace GuelderEngine::Vulkan
{
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