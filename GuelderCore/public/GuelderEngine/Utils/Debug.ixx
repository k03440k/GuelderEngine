module;
#include <Windows.h>
export module GuelderEngine.Debug;

export import GuelderEngine.Core.Types;

import <iostream>;
import <stdexcept>;
import <string_view>;
import <string>;
import <sstream>;
import <chrono>;
import <type_traits>;
import <functional>;

template<typename T>
concept HasOutputOperator = requires(std::stringstream & os, const T & t) { os << t; };

//enums
export namespace GuelderEngine
{
    namespace Debug
    {
        enum class ConsoleForegroundColor : WORD
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

export namespace GuelderEngine::Debug
{
    template<typename... Attributes>
    concept ConsoleColorAttributes = ((std::is_same_v<Attributes, ::GuelderEngine::Debug::ConsoleForegroundColor> || ...)
        || (std::is_same_v<Attributes, ::GuelderEngine::Debug::ConsoleBackgroundColor> || ...));

    enum class LogLevel : ubyte
    {
        Info,
        Warning,
        Error,
        RendererError,
        VulkanError,
        All = Info | Warning | Error | RendererError | RendererError | VulkanError
    };

    ///*template<typename... Attributes>
    //template<Attributes... args>
    //struct LoggingCategoryAttributes
    //{
    //    constexpr WORD attrs = static_cast<WORD>((static_cast<WORD>(args) | ...));
    //};*/

    //template<WORD Attrs>
    //struct LoggingCategoryAttributes
    //{
    //    constexpr WORD attrs = Attrs;
    //};

    //template<typename T, WORD Attrs>
    //concept LoggingCategoryAttributesTrait = (std::is_same_v<T, LoggingCategoryAttributes<Attrs>>);

    //template<typename _Info,
    //         typename _Warning,
    //         typename _Error,
    //         typename _RendererError,
    //         typename _VulkanError
    //>
    ////requires LoggingCategoryAttributesTrait<_Info, _Warning, _Error, _RendererError, _VulkanError>
    //struct LoggingCategoriesAttributes
    //{
    //    using Info = _Info;
    //    using Warning = _Warning;
    //    using Error = _Error;
    //    using RendererError = _RendererError;
    //    using VulkanError = _VulkanError;
    //};

    /*template<typename _LoggingCategory>
    concept LoggingCattegoryTrait = std::is*/

    template<LogLevel LoggingLevels, bool _enable>
    struct LoggingCategory {};//TODO: add colors support to this struct
    template<LogLevel LoggingLevels>
    struct LoggingCategory<LoggingLevels, false>
    {
        LoggingCategory(const std::string_view& name) {}

        static constexpr LogLevel supportedLoggingLevels = LoggingLevels;
        static constexpr bool enable = false;
    };
    template<LogLevel LoggingLevels>
    struct LoggingCategory<LoggingLevels, true>
    {
        LoggingCategory(const std::string_view& name)
            : name(name) {}
        virtual ~LoggingCategory() = default;

        constexpr bool CanSupportLogLevel(const LogLevel& level) const
        {
            if(supportedLoggingLevels == LogLevel::All)
                return true;
            return static_cast<std::underlying_type_t<LogLevel>>(level) &
                static_cast<std::underlying_type_t<LogLevel>>(supportedLoggingLevels);
        }

        static constexpr LogLevel supportedLoggingLevels = LoggingLevels;
        const std::string name;
        static constexpr bool enable = true;
    };
    /*
    * @note I use HANDE but i don't do CloseHandle in dtor
    */
    class Logger
    {
    public:
        Logger() = default;
        ~Logger() = default;

        template<LogLevel LoggingLevels, typename... Args>
        constexpr static void Log(const LoggingCategory<LoggingLevels, true>& category,
            const LogLevel& level, Args&&... args)
        {
            std::ostringstream oss;
            Format(oss, args...);
            const std::string message = oss.str();
            WriteLog<LoggingLevels>(category, level, message);
        }
        //disable if enable == false
        template<LogLevel LoggingLevels, typename... Args>
        constexpr static void Log(const LoggingCategory<LoggingLevels, false>& category, const LogLevel& level, Args&&... args){}

        [[noreturn]]
        static void Throw(const std::string_view& message, const char* fileName, const uint& line);

        [[noreturn]]
        static void Throw(const std::string_view& message);

        /*
        * @return formated std::string from different variables(const Args&... message)
        */
        template<HasOutputOperator ...Args>
        constexpr static std::string Format(Args&&... message)
        {
            std::ostringstream out;
            (out << ... << message);
            return out.str();
        }

        /*if input bool is false, then it will bring throw of runtime_error*/
        static void Assert(const bool& condition, const std::string& message = "", const char* file = __FILE__, const uint& line = __LINE__);

    private:
        template<ConsoleColorAttributes... Attributes>
        constexpr static void SetConsoleColorAttributes(Attributes&&... attrs)
        {
            SetConsoleTextAttribute(console, static_cast<WORD>((static_cast<WORD>(attrs) | ...)));
        }
        template<typename... Args, std::enable_if_t<((std::is_same_v<Args, WORD> || ...)), int> = 0>
        constexpr static void SetConsoleColorAttributes(Args&&... attrs)
        {
            SetConsoleTextAttribute(console, (attrs | ...));
        }

        template<HasOutputOperator T>
        constexpr static void Format(std::ostringstream& oss, const T& arg)
        {
            oss << arg;
        }

        template<HasOutputOperator T, typename... Args>
        constexpr static void Format(std::ostringstream& oss, const T& arg, Args&&... args)//TODO: CHANGE Args&&..
        {
            oss << arg;
            Format(oss, args...);
        }

        static std::mutex logMutex;

        template<LogLevel LoggingLevels>
        static void WriteLog(const LoggingCategory<LoggingLevels, true>& category, const LogLevel& level,
            const std::string_view& message)
        {
            //const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            //struct tm localTime;
            //localtime_s(&localTime, &t);

            //std::cout << std::put_time(&localTime, "%H:%M:%S") << ' '; // print timestamp

            logMutex.lock();

            std::cout << category.name << ": ";

            using Background = ConsoleBackgroundColor;
            using Text = ConsoleForegroundColor;
            if(level == LogLevel::Info && category.CanSupportLogLevel(LogLevel::Info))
            {
                SetConsoleColorAttributes(Background::Cyan, Text::White);
                std::cout << "[INFO]";
                SetConsoleColorAttributes(Text::Cyan, Background::Black);
            }
            else if(level == LogLevel::Warning && category.CanSupportLogLevel(LogLevel::Warning))
            {
                SetConsoleColorAttributes(Background::Yellow, Text::White);
                std::cout << "[WARNING]";
                SetConsoleColorAttributes(Text::Yellow, Background::Black);
            }
            else if(level == LogLevel::Error && category.CanSupportLogLevel(LogLevel::Error))
            {
                SetConsoleColorAttributes(Background::Red, Text::White);
                std::cerr << "[ERROR]";
                SetConsoleColorAttributes(Text::Red, Background::Black);
            }
            else if(level == LogLevel::RendererError && category.CanSupportLogLevel(LogLevel::RendererError))
            {
                SetConsoleColorAttributes(Background::Red, Text::White);
                std::cout << "[RENDERER ERROR]";
                SetConsoleColorAttributes(Text::Magenta, Background::Black);
            }
            else if(level == LogLevel::VulkanError && category.CanSupportLogLevel(LogLevel::VulkanError))
            {
                SetConsoleColorAttributes(Background::Red, Text::White);
                std::cout << "[VULKAN ERROR]";
                SetConsoleColorAttributes(Text::Magenta, Background::Black);
            }
            else
                Throw(Format("Logger::WriteLog: invalid logging level or ", category.name,
                    " doesn't support logging level"), __FILE__, __LINE__);

            std::cout << ": " << message << '\n'/*std::endl*//* << DEFAULT_FONT*/;
            SetConsoleColorAttributes(Text::White, Background::Black);

            logMutex.unlock();
        }

        static HANDLE console;
    };
}

namespace GuelderEngine::Debug
{
    struct CoreLoggingCategory final : LoggingCategory<LogLevel::All, true>
    {
        CoreLoggingCategory() : LoggingCategory("Core") {}
    };
    CoreLoggingCategory coreLoggingCategory;
}

#pragma region logging fuctions
export namespace GuelderEngine
{
    namespace Debug
    {
        /**
        * Prints into cout as custom type.
        * Another form of Logger::Log(color, categoryName, ...)
        */
        template<LogLevel LoggingLevels, bool _enable, typename... Args>
        constexpr void Log(const LoggingCategory<LoggingLevels, _enable>& category, const LogLevel& level, Args&&... info)
        {
            Logger::Log<LoggingLevels>(category, level, info...);//TODO: remade custom log with making macro which will declare new struct which will be inherited from IConsoleCategory with method GetName() (see in the screenshot)
        }
        /**
        * Prints into cout as info.
        * Another form of Logger::Log(LogLevel::Info, ...)
        */
        template<typename... Args>
        constexpr void LogInfo(Args&&... info)
        {
            Logger::Log<CoreLoggingCategory::supportedLoggingLevels>(coreLoggingCategory, LogLevel::Info, info...);
        }

        /**
        * Prints into cout as the warning.
        * Another form of Logger::Log(LogLevel::Warning, ...)
        */
        template<typename ...Args>
        constexpr void LogWarning(Args&&... info)
        {
            Logger::Log<CoreLoggingCategory::supportedLoggingLevels>(coreLoggingCategory, LogLevel::Warning, info...);
        }

        /**
        * Prints into cout as the error.
        * Another form of Logger::Log(LogLevel::Error, ...)
        */
        template<typename ...Args>
        constexpr void LogError(Args&&... info)
        {
            Logger::Log<CoreLoggingCategory::supportedLoggingLevels>(coreLoggingCategory, LogLevel::Error, info...);
        }

        /**
        * Prints into cout as the renderer error.
        * Another form of Logger::Log(LogLevel::RendererError, ...)
        */

        template<typename ...Args>
        constexpr void LogRendererError(Args&&... info)
        {
            Logger::Log<CoreLoggingCategory::supportedLoggingLevels>(coreLoggingCategory, LogLevel::RendererError, info...);
        }

        /**
        * Prints into cout as the vulkan error.
        * Another form of Logger::Log(LogLevel::VulkanError, ...)
        */
        template<typename ...Args>
        constexpr void LogVulkanError(Args&&... info)
        {
            Logger::Log<CoreLoggingCategory::supportedLoggingLevels>(coreLoggingCategory, LogLevel::VulkanError, info...);
        }
    }
}
#pragma endregion