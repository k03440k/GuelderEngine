#include "../includes/GuelderEngine/Application.hpp"

#include "../includes/GuelderEngine/Utils/Debug.hpp"

#include "../headers/Window.hpp"

#include "../../includes/GuelderEngine/Layers/Layer.hpp"

#include <glfw/glfw3.h>

//#include <stdexcept>
#include <string>//<-- added this
#include <vector>
//#include <iostream>

namespace GuelderEngine
{
#pragma region GEApplication
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
    GEApplication::GEApplication(const Utils::ushort& windowWidth, const Utils::ushort& windowHeight,
        const std::string_view& windowTitle, const std::function<void()>& callOnUpdate)
    {
        m_Window = std::make_unique<Window>(Window::WindowData(windowTitle.data(), windowWidth, windowHeight));
        m_Window->SetCallback(BIND_EVENT_FUNC(GEApplication::OnEvent));
        //this->m_CallOnUpdate = callOnUpdate;
    }
    GEApplication::~GEApplication()
    {
        m_Window->~Window();
    }
    void GEApplication::Run()
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
    void GEApplication::Run(const std::function<void()>& callOnUpdate)
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
    void GEApplication::OnEvent(Events::Event& event)
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
#pragma region VulkanManager
    VulkanManager::VulkanManager(const std::string_view& name)
    {
        //Init();
        CreateVkInstance(name.data());
    }
    void VulkanManager::Init()
    {
        VkInit();
    }
    void VulkanManager::VkInit()
    {
        CreateVkInstance("Guelder Engine Editor");
        //CreateVkInstance();
    }
    vk::Instance VulkanManager::CreateVkInstance(const char* const name)
    {
        uint version{};
        vkEnumerateInstanceVersion(&version);
#ifdef DEBUG_VULKAN
        Logger::Log(LogLevel::Info, Logger::Format("System can support Vulkan variant: ", VK_API_VERSION_VARIANT(version), ", ",
            VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version)));
#endif // DEBUG_VULKAN

        vk::ApplicationInfo appInfo(name, 0, "Guelder Engine", version);

        uint glfwExtensionsCount{};
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

#ifdef DEBUG_VULKAN
        if(extensions.size() > 0)
        {
            Logger::Log(LogLevel::Info, "All available extensions for Vulkan are:");
            for (const auto extension : extensions)
            {
                Logger::Log(LogLevel::Info, extension);
            }
        }
#endif // DEBUG_VULKAN

        std::vector<const char*> layers;

#ifdef DEBUG_VULKAN
        layers.push_back("VK_LAYER_KHRONOS_validation");
#endif // DEBUG_VULKAN

        Logger::Assert(IsValidationLayersSupported(layers), "VulkanManager::CreateVkInstance: validation layers are not supported");
        Logger::Assert(IsExtensionsSupported(extensions), MSG_METHOD_LOGGING("extensions are not supported")
            /*"VulkanManager::CreateVkInstance: extensions are not supported"*/);

        vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
            &appInfo,
            layers.size(), layers.data(),//debug layers
            extensions.size(), extensions.data());//extensions

        //m_Instance = vk::createInstance(createInfo);
        return vk::createInstance(createInfo);
    }
    bool VulkanManager::IsExtensionsSupported(const std::vector<const char*>& extensions)
    {
        Logger::Assert(extensions.size() > 0, "VulkanManager::IsValidationLayersSupported: extensions size is zero");

        std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

#ifdef DEBUG_VULKAN
        Logger::Log(LogLevel::Info, "Device can support following extensions:");
        for (const auto& supportedExtension : supportedExtensions)
        {
            Logger::Log(LogLevel::Info, supportedExtension.extensionName);
        }
#endif //DEBUG_VULKAN

        bool found = false;
        for (const auto& extension : extensions)
        {
            found = false;
            for (const auto& supportedExtension : supportedExtensions)
            {
                if (strcmp(extension, supportedExtension.extensionName) == 0)
                {
                    found = true;
#ifdef DEBUG_VULKAN
                    Logger::Log(LogLevel::Info, "Extension \"", extension, "\" is supported");
#endif //DEBUG_VULKAN
                }
            }
            if (!found)
            {
#ifdef DEBUG_VULKAN
                Logger::Log(LogLevel::Info, "Extension \"", extension, "\" is not supported");
#endif //DEBUG_VULKAN
                return false;
            }
        }

        return true;
    }
    bool VulkanManager::IsValidationLayersSupported(const std::vector<const char*>& layers)
    {
        Logger::Assert(layers.size() > 0, "VulkanManager::IsValidationLayersSupported: layers size is zero");

        std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

#ifdef DEBUG_VULKAN
        Logger::Log(LogLevel::Info, "Device can support following layers:");
        for (const auto& layer : supportedLayers)
        {
            Logger::Log(LogLevel::Info, layer.layerName); 
        }
#endif // DEBUG_VULKAN

        bool found = false;
        for (const auto& extension : layers)
        {
            found = false;
            for (const auto& supportedExtension : supportedLayers)
            {
                if (strcmp(extension, supportedExtension.layerName) == 0)
                {
                    found = true;
#ifdef DEBUG_VULKAN
                    Logger::Log(LogLevel::Info, "Layer \"", extension, "\" is supported");
#endif //DEBUG_VULKAN
                }
            }
            if (!found)
            {
#ifdef DEBUG_VULKAN
                Logger::Log(LogLevel::Info, "Layer \"", extension, "\" is not supported");
#endif //DEBUG_VULKAN
                return false;
            }
        }

        return true;
    }
    //    void VulkanManager::CreateVkInstance()
//    {
//#ifdef DEBUG_VULKAN
//        uint version{};
//        vkEnumerateInstanceVersion(&version);
//
//        Logger::Log(LogLevel::Info, Logger::Format("System can support Vulkan variant: ", VK_API_VERSION_VARIANT(version), ", ",
//        VK_API_VERSION_MAJOR(version), '.', VK_API_VERSION_MINOR(version), '.', VK_API_VERSION_PATCH(version)));
//#endif // DEBUG_VULKAN
//
//        VkApplicationInfo appInfo{};
//        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//        appInfo.pApplicationName = "Guelder Engine Editor";
//        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
//        appInfo.pEngineName = "Guelder Engine";
//        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
//        appInfo.apiVersion = VK_API_VERSION_1_3;
//        appInfo.pNext = nullptr;
//
//        VkInstanceCreateInfo createInfo{};
//        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//        createInfo.pApplicationInfo = &appInfo;
//
//        uint glfwExtensionCount{};
//        const char** glfwExtensions;
//
//        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//        createInfo.enabledExtensionCount = glfwExtensionCount;
//        createInfo.ppEnabledExtensionNames = glfwExtensions;
//        createInfo.enabledLayerCount = 0;
//
//        Logger::Assert(vkCreateInstance(&createInfo, nullptr, &m_Instance) == VK_SUCCESS,
//            "VulkanManager::CreateVkInstance: cannot create vulkan instance");
//
//        uint extensionCount = 0;
//
//        std::vector<VkExtensionProperties> extensions(extensionCount);
//
//        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
//
//#ifdef DEBUG_VULKAN
//        if(extensionCount > 0)
//        {
//            Logger::Log(LogLevel::Info, "all available extensions for Vulkan are:");
//            for (const auto& it : extensions)
//            {
//                Logger::Log(LogLevel::Info, it.extensionName);
//            }
//        }
//#endif //DEBUG_VULKAN
//    }
    void VulkanManager::Cleanup()
    {
        m_Instance.destroy();
        //vkDestroyInstance(m_Instance, nullptr);
    }
    VulkanManager::~VulkanManager()
    {
        Cleanup();
    }
#pragma endregion
}