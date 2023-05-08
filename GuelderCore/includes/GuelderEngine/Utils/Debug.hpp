#pragma once

#include "Utils.hpp"

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <type_traits>
//#include <functional>

#include <Windows.h>

//TODO: make normal assert, because it prints the path of this file(debug, path must be of that file which caused an error)
//finish the inheritance of GClass
//rebuild asserts, and other stuff with new marcoses

#define DEBUG_VULKAN
#define GE_DEBUG

#define FUNC_NAME __func__

/*
* @brief Use this macro only in classes which inherited from GuelderEngine::Core::GClass
*/
#define METHOD_NAME ::GuelderEngine::Debug::Logger::Format(GetClassName(), "::", FUNC_NAME)

/*
* @brief Use this macro only for GE_CORE_ASSERT
*
* @example
*/
#define MSG_METHOD_LOGGING(msg) ::GuelderEngine::Debug::Logger::Format(GetClassName(), "::", FUNC_NAME, ": ", msg)

/*
* @brief guelder engine error log
* @param all debug info(it can be chars, ints, floats, and other types which support '<<' operator)
*/
#define GE_ELOG(...) ::GuelderEngine::Debug::Logger::ELog(Debug::Logger::Format(__VA_ARGS__), __FILE__, __LINE__)

/*
* @brief This macro is used in core classes which are inherited from GuelderEngine::Core::GClass.
* If condition(param) is false then it will throw exception. Prints the path of the file and msg(param).
* 
* @param condition - bool
* @param msg - string message
*/
#define GE_CORE_ASSERT(condition, msg) ::GuelderEngine::Debug::Logger::Assert(condition, MSG_METHOD_LOGGING(msg), __FILE__, __LINE__)

/*
* @brief If condition(param) is false then it will throw exception. Prints the path of the file and message(msg).
*
* @param condition - bool
* @param msg - string message
*/
#define GE_ASSERT(condition, ...) ::GuelderEngine::Debug::Logger::Assert(condition, __VA_ARGS__, __FILE__, __LINE__)

template<typename T>
concept HasOutputOperator = requires(std::stringstream & os, const T & t) { os << t; };

namespace GuelderEngine
{
    namespace Debug
    {
        enum class ConsoleTextColor : WORD
        {
            Black = 0,
            Blue = FOREGROUND_BLUE,
            Green = FOREGROUND_GREEN,
            Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
            Red = FOREGROUND_RED,
            Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
            Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
            White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            Gray = FOREGROUND_INTENSITY,
            BrightBlue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            BrightGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            BrightCyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            BrightRed = FOREGROUND_RED | FOREGROUND_INTENSITY,
            BrightMagenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            BrightYellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            BrightWhite = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
        };

        enum class ConsoleBackgroundColor : WORD
        {
            Black = 0,
            Blue = BACKGROUND_BLUE,
            Green = BACKGROUND_GREEN,
            Cyan = BACKGROUND_GREEN | BACKGROUND_BLUE,
            Red = BACKGROUND_RED,
            Magenta = BACKGROUND_RED | BACKGROUND_BLUE,
            Yellow = BACKGROUND_RED | BACKGROUND_GREEN,
            White = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
        };

    }
}

template<typename... Attributes>
concept ConsoleColorAttributes = (std::is_enum_v<Attributes> && ...)
    && ((std::is_same_v<Attributes, GuelderEngine::Debug::ConsoleTextColor> || ...)
        || (std::is_same_v<Attributes, GuelderEngine::Debug::ConsoleBackgroundColor> || ...));

namespace GuelderEngine
{
    namespace Debug
    {
        enum class LogLevel
        {
            Info,
            Warning,
            Error,
            RendererError,
            VulkanError
        };

        /*
        * @note I use HANDE but i don't do CloseHandle in dtor
        */
        class Logger
        {
        public:
            Logger() = default;
            ~Logger() = default;

            template<typename... Args>
            constexpr inline static void Log(LogLevel level, const Args&... args)
            {
                std::ostringstream oss;
                Format(oss, args...);
                const std::string message = oss.str();
                WriteLog(level, message);
            }

            //throws an error
            static void ELog(const std::string_view& message, const char* const fileName, const Utils::uint& line);

            static void ELog(const std::string_view& message);

            /*
            * @return formated std::string from different variables(const Args&... message)
            */
            template<HasOutputOperator ...Args>
            constexpr static std::string Format(const Args&... message)
            {
                std::ostringstream out;
                (out << ... << message);
                return out.str();
            }

            /*if input bool is false, then it will bring throw of runtime_error*/
            static void Assert(const bool& condition, const std::string_view& message = "", const char* const file = __FILE__, const Utils::uint& line = __LINE__);

        private:
            template<ConsoleColorAttributes... Attributes>
            inline static void SetConsoleColorAttributes(Attributes&&... attrs)
            {
                SetConsoleTextAttribute(console, static_cast<WORD>((static_cast<WORD>(attrs) | ...)));
            }

            template<HasOutputOperator T>
            constexpr static void Format(std::ostringstream& oss, const T& arg)
            {
                oss << arg;
            }

            template<HasOutputOperator T, typename... Args>
            constexpr static void Format(std::ostringstream& oss, const T& arg, const Args&... args)
            {
                oss << arg;
                Format(oss, args...);
            }

            static void WriteLog(LogLevel level, const std::string_view& message);

            static HANDLE console;
        };
    }
#pragma region logging fuctions
    namespace Debug
    {
        /*
        * Prints into cout as info.
        * Another form of Logger::Log(LogLevel::Info, ...)
        */
        template<typename ...Args>
        constexpr void LogInfo(Args&&... info)
        {
            Logger::Log(LogLevel::Info, info...);
        }
//#define LogInfo(...) ::GuelderEngine::Debug::Logger::Log(::GuelderEngine::Debug::LogLevel::Info, __VA_ARGS__)

        /*
        * Prints into cout as the warning.
        * Another form of Logger::Log(LogLevel::Warning, ...)
        */
        template<typename ...Args>
        constexpr void LogWarning(Args&&... info)
        {
            Logger::Log(LogLevel::Warning, info...);
        }
        //#define LogWarning(...) ::GuelderEngine::Debug::Logger::Log(::GuelderEngine::Debug::LogLevel::Warning, __VA_ARGS__)

        /*
        * Prints into cout as the error.
        * Another form of Logger::Log(LogLevel::Error, ...)
        */
        template<typename ...Args>
        constexpr void LogError(Args&&... info)
        {
            Logger::Log(LogLevel::Error, info...);
        }
//#define LogError(...) ::GuelderEngine::Debug::Logger::Log(::GuelderEngine::Debug::LogLevel::Error, __VA_ARGS__)

        /*
        * Prints into cout as the renderer error.
        * Another form of Logger::Log(LogLevel::RendererError, ...)
        */

        template<typename ...Args>
        constexpr void LogRendererError(Args&&... info)
        {
            Logger::Log(LogLevel::RendererError, info...);
        }
//#define LogRendererError(...) ::GuelderEngine::Debug::Logger::Log(::GuelderEngine::Debug::LogLevel::RendererError, __VA_ARGS__)

        /*
        * Prints into cout as the vulkan error.
        * Another form of Logger::Log(LogLevel::VulkanError, ...)
        */
        template<typename ...Args>
        constexpr void LogVulkanError(Args&&... info)
        {
            Logger::Log(LogLevel::VulkanError, info...);
        }
//#define LogVulkanError(...)::GuelderEngine::Debug::Logger::Log(::GuelderEngine::Debug::LogLevel::RendererError, __VA_ARGS__)
    }
#pragma endregion
}