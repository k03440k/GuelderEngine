#pragma once

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
//#include <functional>

#define DEBUG_VULKAN
#define GE_DEBUG

//guelder engine error log
#define GE_ELOG(...) Debug::Logger::ELog(Debug::Logger::Format(##__VA_ARGS__), __FILE__)

#define FUNC_NAME __func__
#define METHOD_NAME std::string(typeid(*this).name + FUNC_NAME)
#define MSG_METHOD_LOGGING(msg) std::string(METHOD_NAME + msg)

template<typename T>
concept HasOutputOperator = requires(std::stringstream & os, const T & t) { os << t; };

namespace GuelderEngine
{
    namespace Debug
    {
        constexpr inline const char* ERROR_FONT = "\033[31m";
        constexpr inline const char* WARNING_FONT = "\033[33m";
        constexpr inline const char* INFO_FONT = "\033[36m";

        constexpr inline const char* BACKGROUND_INFO = "\033[46m";
        constexpr inline const char* BACKGROUND_WARNING = "\033[43m";
        constexpr inline const char* BACKGROUND_ERROR = "\033[41m";

        constexpr inline const char* BACKGROUND_BLACK = "\033[40m";
        constexpr inline const char* DEFAULT_FONT = "\033[0m";

        enum class LogLevel
        {
            Info,
            Warning,
            Error
        };

        class Logger
        {
        public:
            Logger() = delete;
            ~Logger() = delete;

            template<typename... Args>
            constexpr inline static void Log(LogLevel level, const Args&... args)
            {
                std::ostringstream oss;
                Format(oss, args...);
                const std::string message = oss.str();
                WriteLog(level, message);
            }

            //throws an error
            static void ELog(const std::string_view& message, const char* file_name)
            {
                //const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                //const auto tm = *std::localtime(&t);
                std::ostringstream out;
                out << BACKGROUND_ERROR << ERROR_FONT << BACKGROUND_BLACK << message << '\n'
                    << "file: " << file_name << DEFAULT_FONT;
                throw(std::runtime_error(out.str()));
            }

            static void ELog(const std::string_view& message)
            {
                std::ostringstream out;
                out << ERROR_FONT << message << DEFAULT_FONT;
                throw(std::runtime_error(out.str()));
            }

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
            static void Assert(const bool& condition, const std::string_view& message = "");

        private:

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
        };
    }
}