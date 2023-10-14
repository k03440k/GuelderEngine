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
    struct WindowSize
    {
        uint width;
        uint height;
    };
    class Window// : INHERIT_GClass(Window)
    {
    public:
        using EventCallbackFunc = std::function<void(Events::BaseEvent&)>;

        struct WindowData
        {
        public:
            WindowData(const ushort& width = 640, const ushort& height = 480, const std::string& title = "GuelderEngine window",
                const bool& showFrameRate = true, const EventCallbackFunc& callback = EventCallbackFunc());
            ~WindowData() = default;
            WindowData& operator=(const WindowData& other);

            void Reset() noexcept;

            ushort width = 0;
            ushort height = 0;
            std::string title;
            EventCallbackFunc callback;
            
            bool showFrameRate : 1 = true;

            float UpdateFrameRate();

            int GetFrameRate() const noexcept;

            void SetSize(const uint& width, const uint& height) noexcept;

        private:
            double m_LastTime, m_CurrentTime;
            int m_NumFrames;
            float m_FrameTime;
            int m_FrameRate;
        };

        DECLARE_COPY_AND_MOVING(Window);
        
        Window(const ushort& windowWidth = 640, const ushort& windowHeight = 480, const std::string& windowTitle = "Guelder Engine Window");
        Window(const WindowData& data);
        virtual ~Window();

        void Reset() noexcept;

        virtual void OnUpdate();

        bool ShouldClose() const;

        const WindowData& GetData() const noexcept;
        GLFWwindow* GetGLFWWindow() const noexcept;

        void SetCallback(const EventCallbackFunc& callback) noexcept;
        void SetWindow(const ushort& windowWidth = 640, const ushort& windowHeight = 480, const std::string& windowTitle = std::string("Guelder Engine Window"));
        void SetWindowSize(const ushort& width = 640, const ushort& height = 480);
        void ShowFrameRate();

        bool& WasWindowResized() noexcept;

    private:
        void Init();
        void Shutdown();

        bool m_WasResized;

        WindowData m_Data;
        GLFWwindow* m_GLFWWindow;
    };
}