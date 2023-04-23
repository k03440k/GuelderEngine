#include "../includes/GuelderEngine/Utils/Debug.hpp"

namespace GuelderEngine
{
    namespace Debug
    {
        void Logger::Assert(const bool& condition, const std::string_view& message)
        {
            if (!condition)
            {
                GR_ELOG(message);
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
                break;
            }
            std::cout << DEFAULT_FONT;
        }
    }
}