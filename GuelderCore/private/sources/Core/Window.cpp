module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#undef IMGUI_IMPL_VULKAN_HAS_DYNAMIC_RENDERING
#include <imgui/imgui.h>
#include <glfw/glfw3.h>
module GuelderEngine.Core;
import :Window;

import GuelderEngine.Core.Types;
import GuelderEngine.Events;
import GuelderEngine.Debug;

import <stdexcept>;
import <thread>;
import <string>;
import <string_view>;

namespace GuelderEngine
{
    Window::WindowData::WindowData(
        const ushort& width,
        const ushort& height,
        const std::string& title,
        const bool& showFrameRate,
        const EventCallbackFunc& callback
    )
        : width(width), height(height), title(title), callback(callback), showFrameRate(showFrameRate),
          m_LastTime(0.0), m_CurrentTime(0.0), m_NumFrames(0), m_FrameTime(0.f), m_FrameRate(0) {}
}
namespace GuelderEngine
{
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
    void Window::WindowData::SetSize(const uint& width, const uint& height) noexcept
    {
        this->width = width;
        this->height = height;
    }
}

namespace GuelderEngine::Events
{
    [[noreturn]]
    void GLFWErrorCallback(int errorCode, const char* description)
    {
        GE_THROW("GLFW error(", errorCode, "), description: ", description);
    }
}

namespace GuelderEngine
{
    bool Window::isGLFWInit = false;

    Window::Window(
        const ushort& windowWidth,
        const ushort& windowHeight,
        const std::string_view& windowTitle
    ) :
        m_Data(windowWidth, windowHeight, windowTitle.data())
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

        //TODO: remake copying
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
}
namespace GuelderEngine
{
    void Window::Reset() noexcept
    {
        m_Data.Reset();
        m_GLFWWindow = nullptr;
    }

    void Window::Init()
    {
        m_WasResized = false;

        InitGLFW();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_GLFWWindow = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);

        if(!m_GLFWWindow)
        {
            glfwTerminate();
            GE_THROW("window is nullptr");
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
    bool Window::IsGLFWInit()
    {
        return isGLFWInit;
    }
    void Window::InitGLFW()
    {
        if(!isGLFWInit)
        {
            if(!glfwInit())
            {
                glfwSetErrorCallback(Events::GLFWErrorCallback);
                glfwTerminate();
                GE_THROW("cannot initialise glfw");
            }

            isGLFWInit = true;
        }
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
}
namespace GuelderEngine
{
    const Window::WindowData& Window::GetData() const noexcept
    {
        return m_Data;
    }
    GLFWwindow* Window::GetGLFWWindow() const noexcept
    {
        return m_GLFWWindow;
    }
    void Window::SetCallback(const EventCallbackFunc& callback) noexcept { m_Data.callback = callback; }
    void Window::SetWindow(const ushort& windowWidth, const ushort& windowHeight, const std::string& windowTitle)
    {
        m_Data = Window::WindowData(windowWidth, windowHeight, windowTitle);

        if(m_GLFWWindow) glfwDestroyWindow(m_GLFWWindow);

        Init();
    }
    void Window::SetWindowSize(const ushort& width, const ushort& height)
    {
        m_Data.width = width;
        m_Data.height = height;
        glfwSetWindowSize(m_GLFWWindow, m_Data.width, m_Data.height);
    }
}