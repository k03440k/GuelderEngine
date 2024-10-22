#pragma once

import GuelderEngine.Debug;

//#define NO_DEBUG

#ifdef _DEBUG
#ifndef NO_DEBUG

#define GE_DEBUG

#endif
#endif

#ifdef GE_DEBUG

#define GE_DEBUG_VULKAN//high fps when disable because vulkan debug manager doesn't work, boost ~1000 fps (*__*)

#endif

#define FUNC_NAME __func__

#ifdef _MSC_VER
#define FULL_FUNC_NAME __FUNCSIG__
#elif __GNUC__ || __clang__
#define FULL_FUNC_NAME __PRETTY_FUNCTION__
#else
#define FULL_FUNC_NAME FUNC_NAME
#endif

#define MSG_METHOD_LOGGING(...) ::GuelderEngine::Debug::Logger::Format(FULL_FUNC_NAME, ": ", __VA_ARGS__)

/// <summary>
/// guelder engine error log
/// </summary>
/// <param name = "...">
/// all debug info(it can be chars, ints, floats, and other types which support '<<' operator)
/// </param>
#define GE_THROW(...) ::GuelderEngine::Debug::Logger::Throw(Debug::Logger::Format(FULL_FUNC_NAME, ": ", __VA_ARGS__), __FILE__, __LINE__)

/// <summary>
/// brief If condition(param) is false then it will throw exception. Prints the path of the file and message(msg).
/// </summary>
/// <param name = "...">
/// message, any type that supports "<<" operator
/// </param>
#define GE_ASSERT(condition, ...) ::GuelderEngine::Debug::Logger::Assert(condition, ::GuelderEngine::Debug::Logger::Format(FULL_FUNC_NAME, ": ", __VA_ARGS__), __FILE__, __LINE__)

#define GE_TO_STRING(arg) #arg
#define CONCATENATE(lhs, rhs) GE_TO_STRING(lhs ## rhs)

#define LOG_CATEGORY_VARIABLE(name) name##LoggingCategoryVar
#define LOG_CATEGORY(name) name##LoggingCategory

#define DECLARE_LOG_CATEGORY_EXTERN(name, loggingLevels, enable, debugOnly)\
    struct LOG_CATEGORY(name) final : ::GuelderEngine::Debug::LoggingCategory<::GuelderEngine::Debug::LogLevel::##loggingLevels, enable, debugOnly>\
    {\
    LOG_CATEGORY(name)() : ::GuelderEngine::Debug::LoggingCategory<::GuelderEngine::Debug::LogLevel::##loggingLevels, enable, debugOnly>(GE_TO_STRING(name)) {}\
    };\
    extern LOG_CATEGORY(name) LOG_CATEGORY_VARIABLE(name)


#ifdef GE_DEBUG
#ifndef NO_DEBUG

#define DEFINE_LOG_CATEGORY(name) LOG_CATEGORY(name) LOG_CATEGORY_VARIABLE(name)

#define GE_LOG(categoryName, level, ...) ::GuelderEngine::Debug::Log<LOG_CATEGORY(categoryName)::enable && !LOG_CATEGORY(categoryName)::debugOnly, LOG_CATEGORY(categoryName)::supportedLoggingLevels, LOG_CATEGORY(categoryName)::enable, LOG_CATEGORY(categoryName)::debugOnly>(LOG_CATEGORY_VARIABLE(categoryName), ::GuelderEngine::Debug::LogLevel::##level, __VA_ARGS__)

#endif
#endif

#ifndef GE_DEBUG
#ifndef NO_DEBUG

#define DEFINE_LOG_CATEGORY(name) LOG_CATEGORY(name) LOG_CATEGORY_VARIABLE(name)

#define GE_LOG(categoryName, level, ...) ::GuelderEngine::Debug::Log<LOG_CATEGORY(categoryName)::enable && !LOG_CATEGORY(categoryName)::debugOnly, LOG_CATEGORY(categoryName)::supportedLoggingLevels, LOG_CATEGORY(categoryName)::enable, LOG_CATEGORY(categoryName)::debugOnly>(LOG_CATEGORY_VARIABLE(categoryName), ::GuelderEngine::Debug::LogLevel::##level, __VA_ARGS__)

#endif
#endif

#ifdef NO_DEBUG

#define DEFINE_LOG_CATEGORY(...)
#define GE_LOG(...)

#endif // NO_DEBUG