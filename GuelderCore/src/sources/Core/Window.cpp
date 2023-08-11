module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include <imgui/imgui.h>
#include <glfw/glfw3.h>
module GuelderEngine.Core;
import :Window;

import GuelderEngine.Core.Types;
import GuelderEngine.Events;
import GuelderEngine.Debug;

import <stdexcept>;

//ctors
namespace GuelderEngine
{
    Window::Window(const Types::ushort& windowWidth, const Types::ushort& windowHeight,
        const std::string& windowTitle, const UpdateFunc& update, const bool& enableVSync) :
        m_Data(windowTitle, windowWidth, windowHeight, enableVSync), onUpdate(update)
    {
        Init();
    }
    Window::Window(const WindowData& data)
    {
        this->m_Data = data;
        Init();
    }
    Window::~Window()
    {
        Shutdown();
    }
}
namespace GuelderEngine
{
    Window::WindowData::WindowData(const std::string& title, const Types::ushort& width,
        const Types::ushort& height, const bool& showFrameRate, const bool& isVSync,
        const EventCallbackFunc& callback) : title(title), width(width), height(height),
        callback(callback), showFrameRate(showFrameRate), isVSync(isVSync) {}
    Window::WindowData& Window::WindowData::operator=(const WindowData& other)
    {
        title = other.title;
        width = other.width;
        height = other.height;
        callback = other.callback;
        isVSync = other.isVSync;

        return *this;
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
}
namespace GuelderEngine
{
    inline bool is_GLFW_init = false;
#pragma region Window
    namespace Events
    {
        static void GLFWErrorCallback(int errorCode, const char* description)
        {
            GE_THROW("GuelderEngine::GLFWErrorCallback: error code: ", errorCode);
        }
    }

    void Window::Init()
    {
        if(!is_GLFW_init)
        {
            if(!glfwInit())
            {
                glfwSetErrorCallback(Events::GLFWErrorCallback);
                glfwTerminate();
                GE_CORE_CLASS_THROW("cannot initialise glfw");
            }

            is_GLFW_init = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_GLFWWindow = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);

        if(!m_GLFWWindow)
        {
            glfwTerminate();
            GE_CORE_CLASS_THROW("window is nullptr");
        }

        glfwMakeContextCurrent(m_GLFWWindow);
        glfwSetWindowUserPointer(m_GLFWWindow, &m_Data);
        SetVSync(m_Data.isVSync);

        //set glfw callbacks
        glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                data.width = width;
                data.height = height;

                Events::WindowResizeEvent event(width, height);
                data.callback(event);
            });
        glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow* window)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                Events::WindowCloseEvent event;
                data.callback(event);
            });
        glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                switch(action)
                {
                case GLFW_PRESS:
                {
                    Events::KeyPressedEvent event(key, 0);
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    Events::KeyReleasedEvent event(key, 0);
                    data.callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    Events::KeyPressedEvent event(key, 1);
                    data.callback(event);
                    break;
                }
                }
            });
        glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                switch(action)
                {
                case GLFW_PRESS:
                {
                    Events::MouseButtonPressedEvent event(button);
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    Events::MouseButtonReleasedEvent event(button);
                    data.callback(event);
                    break;
                }
                }
            });
        glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                Events::MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
                data.callback(event);
            });
        glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow* window, double x, double y)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                Events::MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
                data.callback(event);
            });
    }
    void Window::SetVSync(const bool& isEnable)
    {
        if (isEnable) glfwSwapInterval(1);
        else glfwSwapInterval(0);
        m_Data.isVSync = isEnable;
    }
    void Window::SetWindow(const Types::ushort& windowWidth, const Types::ushort& windowHeight, const std::string& windowTitle)
    {
        m_Data = Window::WindowData(windowTitle, windowWidth, windowHeight);

        if (m_GLFWWindow) glfwDestroyWindow(m_GLFWWindow);

        Init();
    }
    void Window::ShowFrameRate()//TODO: show fps
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
    }void Window::Shutdown()
    {
        m_Data.~WindowData();
        glfwDestroyWindow(m_GLFWWindow);
        glfwTerminate();
    }
    void Window::OnUpdate()
    {
        onUpdate();
        
        ShowFrameRate();

        glfwSwapBuffers(m_GLFWWindow);
        glfwPollEvents();
    }
    void Window::OnUpdate(const UpdateFunc& update)
    {
        update();

        ShowFrameRate();

        glfwSwapBuffers(m_GLFWWindow);
        glfwPollEvents();
    }
    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_GLFWWindow);
    }
#pragma endregion
}