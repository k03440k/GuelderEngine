module;

//#include "../../includes/GuelderEngine/GuelderEngine.hpp"
//#include "../includes/GuelderEngine/Types/Types.hpp"
//#include "../../includes/Engine3DCore/Events/Event.hpp"
//#include <functional>
//#include <string>
#include <glfw/glfw3.h>
export module GuelderEngine.Core:Window;

import <functional>;
import <string>;

import GuelderEngine.Core.Types;
import GuelderEngine.Events;

//struct GLFWwindow;

struct GLFWwindow;

export namespace GuelderEngine
{
    namespace Events { struct Event; }
    class Window
    {
    public:
        using EventCallbackFunc = std::function<void(Events::Event&)>;
        using UpdateFunc = std::function<void()>;

        struct WindowData
        {
            WindowData(const std::string& title = "GuelderEngine window", const Types::ushort& width = 640, const Types::ushort& height = 480,
                const bool isVSync = false, const EventCallbackFunc& callback = EventCallbackFunc());
            ~WindowData();
            WindowData& operator=(const WindowData& other);

            Types::ushort width = 0;
            Types::ushort height = 0;
            std::string title;
            EventCallbackFunc callback;

            bool isVSync = false;
        };

        Window() = default;
        Window(const Types::ushort& windowWidth, const Types::ushort& windowHeight,
            const std::string& windowTitle, const UpdateFunc& update = [] {}, const bool& enableVSync = false);
        Window(const WindowData& data);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        virtual void OnUpdate();
        void OnUpdate(const UpdateFunc& update);

        const bool ShouldClose() const;

        UpdateFunc onUpdate;

        Types::ushort GetWidth() const noexcept { return m_Data.width; }
        Types::ushort GetHeight() const noexcept { return m_Data.height; }
        std::string GetTitle() const noexcept { return m_Data.title; }
        //GLFWwindow* GetWindow() const { return window; }
        const bool isVSync() const noexcept { return m_Data.isVSync; }

        void SetVSync(const bool& isEnable = true);

        void SetCallback(const EventCallbackFunc& callback) noexcept { m_Data.callback = callback; }
        void SetWindow(const Types::ushort& windowWidth = 640, const Types::ushort& windowHeight = 480, const std::string& windowTitle = std::string("window"));
        //void SetWindow(GLFWwindow* other, const std::string* otherTitle = nullptr);
    private:
        void Init();
        void Shutdown();

        WindowData m_Data;
        GLFWwindow* m_Window = nullptr;

        //TEST
        void TEST_DrawImGui();
    };
}