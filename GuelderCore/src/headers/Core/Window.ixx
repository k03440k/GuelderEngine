module;
#include "GObject/GClass.hpp"
#include <glfw/glfw3.h>
export module GuelderEngine.Core:Window;

import <functional>;
import <string>;

import GuelderEngine.Core.Types;
import GuelderEngine.Events;

export namespace GuelderEngine
{
    namespace Events { struct BaseEvent; }
    class Window : INHERIT_GClass(Window)
    {
    public:
        using EventCallbackFunc = std::function<void(Events::BaseEvent&)>;
        using UpdateFunc = std::function<void()>;

        struct WindowData
        {
        public:
            WindowData(const std::string& title = "GuelderEngine window", const Types::ushort& width = 640, const Types::ushort& height = 480,
                const bool& showFrameRate = true, const bool& isVSync = false, const EventCallbackFunc& callback = EventCallbackFunc());
            ~WindowData() = default;
            WindowData& operator=(const WindowData& other);

            Types::ushort width = 0;
            Types::ushort height = 0;
            std::string title;
            EventCallbackFunc callback;

            bool isVSync : 1 = false;
            bool showFrameRate : 1 = true;

            float UpdateFrameRate();

            int GetFrameRate() const noexcept;

        private:
            double m_LastTime, m_CurrentTime;
            int m_NumFrames;
            float m_FrameTime;
            int m_FrameRate;
        };

        DELETE_COPY_AND_MOVING(Window);

        Window() = default;
        Window(const Types::ushort& windowWidth, const Types::ushort& windowHeight,
            const std::string& windowTitle, const UpdateFunc& update = [] {}, const bool& enableVSync = false);
        Window(const WindowData& data);
        virtual ~Window();

        virtual void OnUpdate();
        void OnUpdate(const UpdateFunc& update);

        bool ShouldClose() const;

        Types::ushort GetWidth() const noexcept { return m_Data.width; }
        Types::ushort GetHeight() const noexcept { return m_Data.height; }
        std::string GetTitle() const noexcept { return m_Data.title; }
        //GLFWwindow* GetWindow() const { return window; }
        bool IsVSync() const noexcept { return m_Data.isVSync; }

        void SetVSync(const bool& isEnable = true);

        void SetCallback(const EventCallbackFunc& callback) noexcept { m_Data.callback = callback; }
        void SetWindow(const Types::ushort& windowWidth = 640, const Types::ushort& windowHeight = 480, const std::string& windowTitle = std::string("window"));
        //void SetWindow(GLFWwindow* other, const std::string* otherTitle = nullptr);
        void ShowFrameRate();
        int GetFrameRate() const noexcept { return m_Data.GetFrameRate(); }

        UpdateFunc onUpdate;

    private:
        friend class GEApplication;

        void Init();
        void Shutdown();

        WindowData m_Data;
        GLFWwindow* m_GLFWWindow;
    };
}