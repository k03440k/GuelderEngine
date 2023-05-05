#pragma once

#include "Utils/Utils.hpp"

#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"

//#include "Layers/Layer.hpp"
#include "Layers/LayerStack.hpp"

#include <functional>
#include <memory>
//#include <string>
#include <string_view>//<-- added this

//#include "../src/headers/Window.hpp"

#include <vulkan/vulkan.hpp>

namespace GuelderEngine
{
    //idk is it normal
    //namespace Events { struct WindowCloseEvent; struct Event; class EventDispatcher; }
    namespace Layers { class Layer; class LayerStack; }

    /*
    * Manager which responsible for all about initialization of Vulkan API
    */
    class VulkanManager
    {
    public:
        VulkanManager(const std::string_view& name = "Guelder Engine Editor");
        ~VulkanManager();

        static vk::Instance CreateVkInstance(const char* const name);
        static bool IsExtensionsSupported(const std::vector<const char*>& extensions);
        static bool IsValidationLayersSupported(const std::vector<const char*>& layers);
    protected:
    private:
        void Init();
        void VkInit();
        //void CreateVkInstance();
        void Cleanup();

        vk::Instance m_Instance;
        //VkInstance m_Instance;
    };

    class IApplication
    {
    public:
        ~IApplication() = default;

        virtual void Run() = 0;
        virtual void OnEvent(Events::Event& event) = 0;

    protected:
        std::unique_ptr<class Window> m_Window;
    };

    //Guelder Engine Application
    class GEApplication : public IApplication, public VulkanManager
    {
    public:
        GEApplication(const Utils::ushort& windowWidth = 640, const Utils::ushort& windowHeight = 480,
                    const std::string_view& windowTitle = "Guelder Engine Window", const std::function<void()>& callOnUpdate = [] {});
        virtual ~GEApplication();

        GEApplication(const GEApplication&) = delete;
        GEApplication(GEApplication&&) = delete;
        GEApplication& operator=(const GEApplication&) = delete;
        GEApplication& operator=(GEApplication&&) = delete;

        void Run() override;
        void Run(const std::function<void()>& callOnUpdate);
        void OnEvent(Events::Event& event) override;

        void PushLayer(Layers::Layer* layer);
        void PushOverlay(Layers::Layer* overlay);

        //Window& GetWindow() { return *window; }

        void SetOnUpdateFunc(const std::function<void()>& onUpdate) noexcept;
        const std::function<void()>& GetOnUpdateFunc();

        Events::EventDispatcher eventDispatcher;
    private:
        bool OnWindowCloseEvent(const Events::WindowCloseEvent& event) noexcept;

        //std::function<void()> m_CallOnUpdate;

        //std::unique_ptr<class Window> m_Window;

        Layers::LayerStack m_LayerStack;

        bool m_CloseWindow = false;
    };
}