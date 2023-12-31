module;
#include <ctime>
#include <Windows.h>
#include "../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine;
import GuelderEngine.Debug;
import GuelderEngine.Core.Types;

import <mutex>;
import <iostream>;
import <stdexcept>;
import <string_view>;
import <string>;
import <chrono>;

namespace GuelderEngine
{
    namespace Debug
    {
        std::mutex Logger::logMutex;
        HANDLE Logger::console = GetStdHandle(STD_OUTPUT_HANDLE);
        /*Logger::~Logger()
        {
            if (console != INVALID_HANDLE_VALUE) {
                CloseHandle(console);
            }
        }*/
        void Logger::Throw(const std::string_view& message, const char* const fileName, const uint& line)
        {
            throw(std::runtime_error(Format( message, '\n', "file: ", fileName, ", line: ", line)));
        }
        void Logger::Throw(const std::string_view& message)
        {
            throw(std::runtime_error(message.data()/*Format(ERROR_FONT, message, DEFAULT_FONT)*/));
        }
        void Logger::Assert(const bool& condition, const std::string& message, const char* const file, const uint& line)
        {
            if (!condition)
            {
                Throw(message, file, line);
                __debugbreak();
            }
        }
    }
}