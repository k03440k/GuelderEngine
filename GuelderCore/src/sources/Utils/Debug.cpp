#include "../includes/GuelderEngine/Utils/Debug.hpp"

namespace GuelderEngine
{
    namespace Debug
    {
        void Logger::ELog(const std::string_view& message, const char* const fileName, const Utils::uint& line)
        {
            //const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            //const auto tm = *std::localtime(&t);
            //std::ostringstream out;
            //out << BACKGROUND_ERROR << ERROR_FONT << BACKGROUND_BLACK << message << '\n'
            //    << "file: " << fileName << DEFAULT_FONT;
            
            throw(std::runtime_error(Format(ERROR_FONT, BACKGROUND_BLACK, message, '\n', "file: ", fileName, ", line: ", line, DEFAULT_FONT)));
        }
        void Logger::ELog(const std::string_view& message)
        {
            //std::ostringstream out;
            //out << ERROR_FONT << message << DEFAULT_FONT;
            
            throw(std::runtime_error(Format(ERROR_FONT, message, DEFAULT_FONT)));
        }
        void Logger::Assert(const bool& condition, const std::string_view& message, const char* const file, const Utils::uint& line)
        {
            if (!condition)
            {
                ELog(message, file, line);
                __debugbreak();
            }
        }
        void Logger::WriteLog(LogLevel level, const std::string_view& message)
        {
            const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            const auto tm = *std::localtime(&t);

            std::cout << std::put_time(&tm, "%H:%M:%S") << ' '; // print timestamp
            switch (level)
            {
            case LogLevel::Info:
                std::cout << BACKGROUND_INFO << "[INFO]" << BACKGROUND_BLACK << INFO_FONT << ": " << message << std::endl;
                break;
            case LogLevel::Warning:
                std::cout << BACKGROUND_WARNING << "[WARNING]" << BACKGROUND_BLACK << WARNING_FONT << ": " << message << std::endl;
                break;
            case LogLevel::Error:
                std::cerr << BACKGROUND_ERROR << "[ERROR]" << BACKGROUND_BLACK << ERROR_FONT << ": " << message << std::endl;
            case LogLevel::RendererError:
                std::cerr << BACKGROUND_ERROR << "[RENDERER ERRROR]" << BACKGROUND_BLACK << RENDERER_ERROR_FONT << ": " << message << std::endl;
                break;
            case LogLevel::VulkanError:
                std::cerr << BACKGROUND_ERROR << "[VULKAN ERRROR]" << BACKGROUND_BLACK << VULKAN_ERROR_FONT << ": " << message << std::endl;
                break;
            }
            std::cout << DEFAULT_FONT;
        }
    }
}