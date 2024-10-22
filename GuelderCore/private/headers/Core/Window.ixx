module;
#include "GObject/GClass.hpp"
#include <glfw/glfw3.h>
export module GuelderEngine.Core:Window;

import <functional>;
import <string>;
import <string_view>;

import GuelderEngine.Core.Types;
import GuelderEngine.Events;

export namespace GuelderEngine
{
    struct WindowSize
    {
        uint width;
        uint height;
    };
    class Window
    {
    public:
        using EventCallbackFunc = std::function<void(Events::BaseEvent&)>;
    public:

        struct WindowData
        {
        public:
            DECLARE_DEFAULT_COPYING_AND_MOVING(WindowData);

            WindowData(
                const ushort& width = 640,
                const ushort& height = 480,
                const std::string& title = "GuelderEngine window",
                const bool& showFrameRate = true,
                const EventCallbackFunc& callback = EventCallbackFunc()
            );
            ~WindowData() = default;

            void Reset() noexcept;

            float UpdateFrameRate();

            int GetFrameRate() const noexcept;
            void SetSize(const uint& width, const uint& height) noexcept;

            ushort width = 0;
            ushort height = 0;
            std::string title;
            EventCallbackFunc callback;

            bool showFrameRate : 1 = true;

        private:
            double m_LastTime, m_CurrentTime;
            int m_NumFrames;
            float m_FrameTime;
            int m_FrameRate;
        };

        DECLARE_COPYING_AND_MOVING(Window);

        Window(const ushort& windowWidth = 640, const ushort& windowHeight = 480, const std::string_view& windowTitle = "Guelder Engine Window");
        Window(const WindowData& data);
        virtual ~Window();

        void Reset() noexcept;

        virtual void OnUpdate();

        bool ShouldClose() const;

        const WindowData& GetData() const noexcept;
        GLFWwindow* GetGLFWWindow() const noexcept;

        void SetCallback(const EventCallbackFunc& callback) noexcept;
        void SetWindow(const ushort& windowWidth = 640, const ushort& windowHeight = 480, const std::string& windowTitle = "Guelder Engine Window");
        void SetWindowSize(const ushort& width = 640, const ushort& height = 480);
        void ShowFrameRate();

        bool& WasWindowResized() noexcept;

        static bool IsGLFWInit();
        static void InitGLFW();

    private:
        void Init();
        void Shutdown();

        static bool isGLFWInit;

        bool m_WasResized;

        WindowData m_Data;
        GLFWwindow* m_GLFWWindow;
    };
}