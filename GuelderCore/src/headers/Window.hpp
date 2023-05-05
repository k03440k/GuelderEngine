#pragma once

#include "../../includes/GuelderEngine/GuelderEngine.hpp"
#include "../includes/GuelderEngine/Utils/Utils.hpp"
//#include "../../includes/Engine3DCore/Events/Event.hpp"
#include <functional>
#include <string>

struct GLFWwindow;

namespace GuelderEngine
{
    namespace Events { struct Event; }
    class Window
    {
    public:
        using EventCallbackFunc = std::function<void(Events::Event&)>;
        using UpdateFunc = std::function<void()>;

        struct WindowData
        {
            WindowData(const std::string& title = "GuelderEngine window", const Utils::ushort& width = 640, const Utils::ushort& height = 480,
                  const bool isVSync = false, const EventCallbackFunc& callback = EventCallbackFunc());
            ~WindowData();
            WindowData& operator=(const WindowData& other);

            Utils::ushort width = 0;
            Utils::ushort height = 0;
            std::string title;
            EventCallbackFunc callback;

            bool isVSync = false;
        };

        Window() = default;
        Window(const Utils::ushort& windowWidth, const Utils::ushort& windowHeight,
               const std::string& windowTitle, const UpdateFunc& update = []{}, const bool& enableVSync = false);
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

        Utils::ushort GetWidth() const noexcept { return m_Data.width; }
        Utils::ushort GetHeight() const noexcept { return m_Data.height; }
        std::string GetTitle() const noexcept { return m_Data.title; }
        //GLFWwindow* GetWindow() const { return window; }
        const bool isVSync() const noexcept { return m_Data.isVSync; }

        void SetVSync(const bool& isEnable = true);

        void SetCallback(const EventCallbackFunc& callback) noexcept { m_Data.callback = callback; }
        void SetWindow(const Utils::ushort& windowWidth = 640, const Utils::ushort& windowHeight = 480, const std::string& windowTitle = std::string("window"));
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