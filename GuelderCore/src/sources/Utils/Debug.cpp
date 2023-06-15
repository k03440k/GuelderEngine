//#include "../includes/GuelderEngine/Utils/Debug.hpp"

//#include <Windows.h>
module;
#include <ctime>
#include <Windows.h>
#include "../includes/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine;
import GuelderEngine.Debug;
import GuelderEngine.Core.Types;

import <iostream>;
import <stdexcept>;
import <string_view>;
import <string>;
import <chrono>;

namespace GuelderEngine
{
    namespace Debug
    {
        HANDLE Logger::console = GetStdHandle(STD_OUTPUT_HANDLE);
        /*Logger::~Logger()
        {
            if (console != INVALID_HANDLE_VALUE) {
                CloseHandle(console);
            }
        }*/
        void Logger::ELog(const std::string_view& message, const char* const fileName, const Types::uint& line)
        {
            //const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            //const auto tm = *std::localtime(&t);
            //std::ostringstream out;
            //out << BACKGROUND_ERROR << ERROR_FONT << BACKGROUND_BLACK << message << '\n'
            //    << "file: " << fileName << DEFAULT_FONT;
            
            throw(std::runtime_error(Format(/*ERROR_FONT, BACKGROUND_BLACK,*/ message, '\n', "file: ", fileName, ", line: ", line/*, DEFAULT_FONT*/)));
        }
        void Logger::ELog(const std::string_view& message)
        {
            //std::ostringstream out;
            //out << ERROR_FONT << message << DEFAULT_FONT;
            
            throw(std::runtime_error(message.data()/*Format(ERROR_FONT, message, DEFAULT_FONT)*/));
        }
        void Logger::Assert(const bool& condition, const std::string_view& message, const char* const file, const Types::uint& line)
        {
            if (!condition)
            {
                ELog(message, file, line);
                __debugbreak();
            }
        }
        void Logger::WriteLog(const LogLevel& level, const std::string_view& message)
        {
            const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            struct tm local_time;
            localtime_s(&local_time, &t);

            std::cout << std::put_time(&local_time, "%H:%M:%S") << ' '; // print timestamp

            using Background = ConsoleBackgroundColor;
            using Text = ConsoleTextColor;
            switch (level)
            {
            case LogLevel::Info:
                //std::cout << BACKGROUND_INFO << "[INFO]" << BACKGROUND_BLACK << INFO_FONT;
                SetConsoleColorAttributes(Background::Cyan, Text::White);
                std::cout << "[INFO]";
                SetConsoleColorAttributes(Text::Cyan, Background::Black);
                break;
            case LogLevel::Warning:
                //std::cout << BACKGROUND_WARNING << "[WARNING]" << BACKGROUND_BLACK << WARNING_FONT;
                SetConsoleColorAttributes(Background::Yellow, Text::White);
                std::cout << "[WARNING]";
                SetConsoleColorAttributes(Text::Yellow, Background::Black);
                break;
            case LogLevel::Error:
                //std::cerr << BACKGROUND_ERROR << "[ERROR]" << BACKGROUND_BLACK << ERROR_FONT;
                SetConsoleColorAttributes(Background::Red, Text::White);
                std::cerr << "[ERROR]";
                SetConsoleColorAttributes(Text::Red, Background::Black);
                break;
            case LogLevel::RendererError:
                //std::cerr << BACKGROUND_ERROR << "[RENDERER ERRROR]" << BACKGROUND_BLACK << RENDERER_ERROR_FONT;
                SetConsoleColorAttributes(Background::Red, Text::White);
                std::cout << "[RENDERER ERRROR]";
                SetConsoleColorAttributes(Text::Magenta, Background::Black);
                break;
            case LogLevel::VulkanError:
                //::cerr << BACKGROUND_ERROR << "[VULKAN ERRROR]" << BACKGROUND_BLACK << VULKAN_ERROR_FONT;
                SetConsoleColorAttributes(Background::Red, Text::White);
                std::cout << "[VULKAN ERRROR]";
                SetConsoleColorAttributes(Text::Magenta, Background::Black);
                break;
            default:
                GE_ELOG("Logger::WriteLog: invalid logging level");
            }
            std::cout << ": " << message << '\n'/*std::endl*//* << DEFAULT_FONT*/;
            SetConsoleColorAttributes(Text::White, Background::Black);
        }
        void Logger::WriteLog(ConsoleTextColor&& textColor, const std::string_view& categoryName,
            const std::string_view& message)
        {
            const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            struct tm local_time;
            localtime_s(&local_time, &t);

            std::cout << std::put_time(&local_time, "%H:%M:%S") << ' '; // print timestamp
            using Background = ConsoleBackgroundColor;
            using Text = ConsoleTextColor;

            SetConsoleColorAttributes(Background::Green, Text::White);

            std::cout << '[' << categoryName << ']';
            SetConsoleColorAttributes(Text::White, Background::Black);
            std::cout << ": ";
            SetConsoleColorAttributes(std::move(textColor));
            std::cout << message << '\n';

            SetConsoleColorAttributes(Text::White, Background::Black);
        }
    }
}