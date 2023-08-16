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
    class Window : INHERIT_GClass(Window)
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

        DELETE_COPY_AND_MOVING(Window);

        //Window() = default;
        Window(const Types::ushort& windowWidth = 640, const Types::ushort& windowHeight = 480, const std::string& windowTitle = "Guelder Engine Window");
        Window(const WindowData& data);
        virtual ~Window();

        virtual void OnUpdate();
        //void OnUpdate(const UpdateFunc& update);

        bool ShouldClose() const;

        Types::ushort GetWidth() const noexcept;
        Types::ushort GetHeight() const noexcept;
        WindowSize GetWindowSize() const noexcept;
        std::string GetTitle() const noexcept;

        void SetCallback(const EventCallbackFunc& callback) noexcept { m_Data.callback = callback; }
        void SetWindow(const Types::ushort& windowWidth = 640, const Types::ushort& windowHeight = 480, const std::string& windowTitle = std::string("Guelder Engine Window"));
        void SetWindowSize(const Types::ushort& width = 640, const Types::ushort& height = 480);
        //void SetWindow(GLFWwindow* other, const std::string* otherTitle = nullptr);
        void ShowFrameRate();
        int GetFrameRate() const noexcept { return m_Data.GetFrameRate(); }

    private:
        friend class GEApplication;

        void Init();
        void Shutdown();

        void UpdateSize();

        WindowData m_Data;
        GLFWwindow* m_GLFWWindow;
    };
}