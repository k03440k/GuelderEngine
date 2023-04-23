#include "../headers/Window.hpp"

#include "../../includes/GuelderEngine/Utils/Debug.hpp"

#include "../../includes/GuelderEngine/Events/Event.hpp"
#include "../../includes/GuelderEngine/Events/ApplicationEvent.hpp"
#include "../../includes/GuelderEngine/Events/MouseEvent.hpp"
#include "../../includes/GuelderEngine/Events/KeyEvent.hpp"

#include <glfw/glfw3.h>
#include <stdexcept>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace GuelderEngine
{
    inline bool is_GLFW_init = false;
#pragma region Window
    namespace Events
    {
        static void GLFWErrorCallback(int errorCode, const char* description)
        {
            GR_ELOG("GuelderEngine::GLFWErrorCallback: error code: ", errorCode);
        }
    }
    //TEST
    void Window::TEST_DrawImGui()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(GetWidth());
        io.DisplaySize.y = static_cast<float>(GetHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void ImGui_Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
    }
    Window::Window(const Utils::ushort& windowWidth, const Utils::ushort& windowHeight,
        const std::string& windowTitle, const UpdateFunc& update, const bool& enableVSync) :
        m_Data(windowTitle, windowWidth, windowHeight, enableVSync), onUpdate(update)
    {
        Init();

        ImGui_Init();
    }
    Window::Window(const WindowData& data)
    {
        this->m_Data = data;
        Init();

        ImGui_Init();
    }
    Window::~Window()
    {
        Shutdown();
    }
    void Window::SetVSync(const bool& isEnable)
    {
        if (isEnable) glfwSwapInterval(1);
        else glfwSwapInterval(0);
        m_Data.isVSync = isEnable;
    }
    void Window::SetWindow(const Utils::ushort& windowWidth, const Utils::ushort& windowHeight, const std::string& windowTitle)
    {
        m_Data = Window::WindowData(windowTitle, windowWidth, windowHeight);

        if (m_Window) glfwDestroyWindow(m_Window);

        Init();
    }
    void Window::Init()
    {
        if (!is_GLFW_init)
        {
            if (!glfwInit())
            {
                glfwSetErrorCallback(Events::GLFWErrorCallback);
                glfwTerminate();
                GR_ELOG("Window::Init: cannot initialise glfw");
            }
            is_GLFW_init = true;
        }

        m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), NULL, NULL);

        if (!m_Window)
        {
            glfwTerminate();
            GR_ELOG("Window::Init: window is nullptr");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(m_Data.isVSync);

        //VulkanInit();

        //set glfw callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.width = width;
                data.height = height;

                Events::WindowResizeEvent event(width, height);
                data.callback(event);
            });
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                Events::WindowCloseEvent event;
                data.callback(event);
            });
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
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
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
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
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                Events::MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.callback(event);
            });
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Events::MouseMovedEvent event((float)x, (float)y);
                data.callback(event);
            });
    }
    void Window::Shutdown()
    {
        m_Data.~WindowData();
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
    void Window::OnUpdate()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        onUpdate();

        TEST_DrawImGui();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
    void Window::OnUpdate(const UpdateFunc& update)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        update();

        TEST_DrawImGui();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
    const bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }
#pragma endregion
#pragma region WindowData
    Window::WindowData::WindowData(const std::string& title, const Utils::ushort& width,
        const Utils::ushort& height, const bool isVSync,
        const EventCallbackFunc& callback) : title(title), width(width), height(height),
        callback(callback), isVSync(isVSync) {}
    Window::WindowData::~WindowData()
    {
        title = "";
        width = 0;
        height = 0;
    }
    Window::WindowData& Window::WindowData::operator=(const WindowData& other)
    {
        title = other.title;
        width = other.width;
        height = other.height;
        callback = other.callback;
        isVSync = other.isVSync;

        return *this;
    }
#pragma endregion
}