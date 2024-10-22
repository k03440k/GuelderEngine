module;
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
module GuelderEngine.UserInterface;
import :UserInterfaceManager;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;

namespace GuelderEngine::UserInterface
{
    using namespace Vulkan;

    void CheckVulkanResult(VkResult result)
    {
        if(result == 0)
            return;
        GE_LOG(VulkanCore, VulkanError, "ImGui thrown an error. VkResult = ", result, ".");
    }
}
namespace GuelderEngine::UserInterface
{
    using namespace Vulkan;

    UserInterfaceManager::UserInterfaceManager(
        GLFWwindow* _GLFWwindow,
        const vk::Instance& instance,
        const vk::PhysicalDevice& physicalDevice,
        const vk::Device& device,
        const vk::RenderPass& renderPass,
        const Vulkan::QueueFamilyIndices& indices,
        const uint& countOfImages,
        const uint& minImageCount,
        const vk::Queue& queue
    )
    {
        std::vector<vk::DescriptorPoolSize> poolSizes = {
                { vk::DescriptorType::eSampler, 1000 },
                { vk::DescriptorType::eCombinedImageSampler, 1000 },
                { vk::DescriptorType::eSampledImage, 1000 },
                { vk::DescriptorType::eStorageImage, 1000 },
                { vk::DescriptorType::eUniformTexelBuffer, 1000 },
                { vk::DescriptorType::eStorageTexelBuffer, 1000 },
                { vk::DescriptorType::eUniformBuffer, 1000 },
                { vk::DescriptorType::eStorageBuffer, 1000 },
                { vk::DescriptorType::eUniformBufferDynamic, 1000 },
                { vk::DescriptorType::eStorageBufferDynamic, 1000 },
                { vk::DescriptorType::eInputAttachment, 1000 }
        };
        m_DescriptorPool = DescriptorPool(
            device,
            poolSizes,
            1000 * IM_ARRAYSIZE(poolSizes.data()),
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
        );

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        GE_ASSERT(ImGui_ImplGlfw_InitForVulkan(_GLFWwindow, true), "Failed to initialize GLFW for Vulkan via ImGui.");

        ImGui_ImplVulkan_InitInfo initInfo;
        initInfo.Instance = instance;
        initInfo.PhysicalDevice = physicalDevice;
        initInfo.Device = device;
        initInfo.QueueFamily = indices.GetGraphicsFamily();
        initInfo.Queue = queue;
        initInfo.PipelineCache = nullptr;
        initInfo.DescriptorPool = m_DescriptorPool.GetDescriptorPool();
        initInfo.Allocator = nullptr;
        initInfo.MinImageCount = minImageCount;
        initInfo.ImageCount = countOfImages;
        initInfo.CheckVkResultFn = CheckVulkanResult;
        initInfo.RenderPass = renderPass;
        initInfo.UseDynamicRendering = false;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Subpass = 0;
        initInfo.MinAllocationSize = 1024 * 1024;
        initInfo.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
        initInfo.PipelineRenderingCreateInfo.pNext = nullptr;
        initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 0;

        ImGui_ImplVulkan_Init(&initInfo);

        ImGui_ImplVulkan_CreateFontsTexture();
    }
    UserInterfaceManager::UserInterfaceManager(UserInterfaceManager&& other) noexcept
    {
        m_DescriptorPool = std::move(other.m_DescriptorPool);

        other.Reset();
    }
    UserInterfaceManager& UserInterfaceManager::operator=(UserInterfaceManager&& other) noexcept
    {
        m_DescriptorPool = std::move(other.m_DescriptorPool);

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::UserInterface
{
    void UserInterfaceManager::Reset() noexcept
    {
        m_DescriptorPool.Reset();
    }
    void UserInterfaceManager::Cleanup(const vk::Device& device) const noexcept
    {
        ImGui_ImplVulkan_Shutdown();
        m_DescriptorPool.Cleanup(device);
    }

    void UserInterfaceManager::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
    }
    void UserInterfaceManager::RenderFrame(const vk::CommandBuffer& commandBuffer)
    {
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }
}
namespace GuelderEngine::UserInterface
{
    const Vulkan::DescriptorPool& UserInterfaceManager::GetDescriptorPool() const
    {
        return m_DescriptorPool;
    }
}