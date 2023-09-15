module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include <imgui/imgui.h>
#include <glfw/glfw3.h>
module GuelderEngine.Core;
import :Window;

import GuelderEngine.Core.Types;
import GuelderEngine.Events;
import GuelderEngine.Debug;

import <stdexcept>;
import <thread>;

namespace GuelderEngine
{
    Window::WindowData::WindowData(const Types::ushort& width,
        const Types::ushort& height, const std::string& title, const bool& showFrameRate,
        const EventCallbackFunc& callback) : title(title), width(width), height(height),
        callback(callback), showFrameRate(showFrameRate) {}
    Window::WindowData& Window::WindowData::operator=(const WindowData& other)
    {
        title = other.title;
        width = other.width;
        height = other.height;
        callback = other.callback;

        return *this;
    }
    void Window::WindowData::Reset() noexcept
    {
        width = 0;
        height = 0;
        title.clear();
        callback = {};
        showFrameRate = true;
        m_LastTime = 0;
        m_CurrentTime = 0;
        m_NumFrames = 0;
        m_FrameTime = 0.f;
        m_FrameRate = 0;
    }
    float Window::WindowData::UpdateFrameRate()
    {
        m_CurrentTime = glfwGetTime();
        const double delta = m_CurrentTime - m_LastTime;

        if(delta >= 1)
        {
            m_FrameRate = std::max(1, static_cast<int>(m_NumFrames / delta));
            m_LastTime = m_CurrentTime;
            m_NumFrames = -1;
            m_FrameTime = static_cast<float>(1000.0 / m_FrameRate);
        }

        ++m_NumFrames;

        return m_FrameRate;
    }
    int Window::WindowData::GetFrameRate() const noexcept
    {
        return m_FrameRate;
    }
    void Window::WindowData::SetSize(const Types::uint& width, const Types::uint& height) noexcept
    {
        this->width = width;
        this->height = height;
    }
}
//ctors
namespace GuelderEngine
{
    bool is_GLFW_init = false;

    namespace Events
    {
        [[noreturn]]
        void GLFWErrorCallback(int errorCode, const char* description)
        {
            GE_THROW("GLFW error(", errorCode, "), description: ", description);
        }
    }
    Window::Window(const Types::ushort& windowWidth, const Types::ushort& windowHeight,
        const std::string& windowTitle) :
        m_Data(windowWidth, windowHeight, windowTitle)
    {
        Init();
    }
    Window::Window(const WindowData& data)
    {
        this->m_Data = data;
        Init();
    }
    Window::Window(const Window& other)
    {
        m_Data = other.m_Data;
        m_GLFWWindow = other.m_GLFWWindow;
        m_WasResized = other.m_WasResized;
    }
    Window::Window(Window&& other) noexcept
    {
        m_Data = other.m_Data;
        m_GLFWWindow = other.m_GLFWWindow;
        m_WasResized = other.m_WasResized;

        other.Reset();
    }
    Window& Window::operator=(const Window& other)
    {
        m_Data = other.m_Data;
        m_WasResized = other.m_WasResized;

        if(m_GLFWWindow != other.m_GLFWWindow && m_GLFWWindow)
            glfwDestroyWindow(m_GLFWWindow);

        m_GLFWWindow = other.m_GLFWWindow;

        return *this;
    }
    Window& Window::operator=(Window&& other) noexcept
    {
        m_Data = other.m_Data;
        m_WasResized = other.m_WasResized;

        if(m_GLFWWindow != other.m_GLFWWindow && m_GLFWWindow)
            glfwDestroyWindow(m_GLFWWindow);

        m_GLFWWindow = other.m_GLFWWindow;

        other.Reset();

        return *this;
    }
    Window::~Window()
    {
        Shutdown();
    }
    void Window::Reset() noexcept
    {
        m_Data.Reset();
        m_GLFWWindow = nullptr;
    }
    void Window::Init()
    {
        m_WasResized = false;

        if(!is_GLFW_init)
        {
            if(!glfwInit())
            {
                glfwSetErrorCallback(Events::GLFWErrorCallback);
                glfwTerminate();
                GE_CLASS_THROW("cannot initialise glfw");
            }

            is_GLFW_init = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_GLFWWindow = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);

        if(!m_GLFWWindow)
        {
            glfwTerminate();
            GE_CLASS_THROW("window is nullptr");
        }

        glfwMakeContextCurrent(m_GLFWWindow);
        glfwSetWindowUserPointer(m_GLFWWindow, this);

        //set glfw callbacks
        glfwSetFramebufferSizeCallback(m_GLFWWindow, [](GLFWwindow* window, int width, int height)
            {
                auto& _window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
                _window.m_WasResized = true;
                _window.m_Data.width = width;
                _window.m_Data.height = height;

                Events::WindowResizeEvent event(width, height);
                _window.m_Data.callback(event);
            });
        glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow* window)
            {
                Window& _window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
                Events::WindowCloseEvent event;
                _window.m_Data.callback(event);
            });
        glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                Window& _window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
                switch(action)
                {
                case GLFW_PRESS:
                {
                    Events::KeyPressedEvent event(key, 0);
                    _window.m_Data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    Events::KeyReleasedEvent event(key, 0);
                    _window.m_Data.callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    Events::KeyPressedEvent event(key, 1);
                    _window.m_Data.callback(event);
                    break;
                }
                }
            });
        glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow* window, int button, int action, int mods)
            {
                Window& _window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
                switch(action)
                {
                case GLFW_PRESS:
                {
                    Events::MouseButtonPressedEvent event(button);
                    _window.m_Data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    Events::MouseButtonReleasedEvent event(button);
                    _window.m_Data.callback(event);
                    break;
                }
                }
            });
        glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                Window& data = *static_cast<Window*>(glfwGetWindowUserPointer(window));
                Events::MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
                data.m_Data.callback(event);
            });
        glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow* window, double x, double y)
            {
                Window& data = *static_cast<Window*>(glfwGetWindowUserPointer(window));

                Events::MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
                data.m_Data.callback(event);
            });
    }
    void Window::Shutdown()
    {
        m_Data.~WindowData();
        glfwDestroyWindow(m_GLFWWindow);
        glfwTerminate();
    }
}
namespace GuelderEngine
{
#pragma region Window

    void Window::ShowFrameRate()
    {
        if(m_Data.showFrameRate)
        {
            const auto fpsTitle = Debug::Logger::Format(m_Data.title, " | fps: ", m_Data.UpdateFrameRate());
            glfwSetWindowTitle(m_GLFWWindow, fpsTitle.c_str());
        }
        else
        {
            glfwSetWindowTitle(m_GLFWWindow, m_Data.title.c_str());
        }
    }
    bool& Window::WasWindowResized() noexcept
    {
        return m_WasResized;
    }
    void Window::OnUpdate()
    {
        if(m_Data.width != 0 && m_Data.height != 0)
            ShowFrameRate();

        //glfwSwapBuffers(m_GLFWWindow);
        glfwPollEvents();//TODO: smooth window resizing
    }
    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_GLFWWindow);
    }
    const Window::WindowData& Window::GetData() const noexcept
    {
        return m_Data;
    }
    GLFWwindow* Window::GetGLFWWindow() const noexcept
    {
        return m_GLFWWindow;
    }
    void Window::SetCallback(const EventCallbackFunc& callback) noexcept { m_Data.callback = callback; }
    void Window::SetWindow(const Types::ushort& windowWidth, const Types::ushort& windowHeight, const std::string& windowTitle)
    {
        m_Data = Window::WindowData(windowWidth, windowHeight, windowTitle);

        if(m_GLFWWindow) glfwDestroyWindow(m_GLFWWindow);

        Init();
    }
    void Window::SetWindowSize(const Types::ushort& width, const Types::ushort& height)
    {
        m_Data.width = width;
        m_Data.height = height;
        glfwSetWindowSize(m_GLFWWindow, m_Data.width, m_Data.height);
    }
#pragma endregion
}