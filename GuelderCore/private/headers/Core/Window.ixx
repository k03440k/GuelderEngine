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
        Types::uint width;
        Types::uint height;
    };
    class Window// : INHERIT_GClass(Window)
    {
    public:
        using EventCallbackFunc = std::function<void(Events::BaseEvent&)>;

        struct WindowData
        {
        public:
            WindowData(const Types::ushort& width = 640, const Types::ushort& height = 480, const std::string& title = "GuelderEngine window",
                const bool& showFrameRate = true, const EventCallbackFunc& callback = EventCallbackFunc());
            ~WindowData() = default;
            WindowData& operator=(const WindowData& other);

            void Reset() noexcept;

            Types::ushort width = 0;
            Types::ushort height = 0;
            std::string title;
            EventCallbackFunc callback;
            
            bool showFrameRate : 1 = true;

            float UpdateFrameRate();

            int GetFrameRate() const noexcept;

            void SetSize(const Types::uint& width, const Types::uint& height) noexcept;

        private:
            double m_LastTime, m_CurrentTime;
            int m_NumFrames;
            float m_FrameTime;
            int m_FrameRate;
        };

        DECLARE_COPY_AND_MOVING(Window);
        
        Window(const Types::ushort& windowWidth = 640, const Types::ushort& windowHeight = 480, const std::string& windowTitle = "Guelder Engine Window");
        Window(const WindowData& data);
        virtual ~Window();

        void Reset() noexcept;

        virtual void OnUpdate();

        bool ShouldClose() const;

        const WindowData& GetData() const noexcept;
        GLFWwindow* GetGLFWWindow() const noexcept;

        void SetCallback(const EventCallbackFunc& callback) noexcept;
        void SetWindow(const Types::ushort& windowWidth = 640, const Types::ushort& windowHeight = 480, const std::string& windowTitle = std::string("Guelder Engine Window"));
        void SetWindowSize(const Types::ushort& width = 640, const Types::ushort& height = 480);
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