#pragma once

import GuelderEngine.Debug;

#define GE_DEBUG

#ifdef GE_DEBUG

#define GE_DEBUG_VULKAN//high fps when disable because vulkan debug manager doesn't work, boost ~1000 fps (*__*)

#endif

#define FUNC_NAME __func__

/**
* @brief Use this macro only in classes which inherited from GuelderEngine::Core::GClass
*/
#define METHOD_NAME ::GuelderEngine::Debug::Logger::Format(GGetClassName(), "::", FUNC_NAME)

/**
* @brief Use this macro only for GE_CORE_CLASS_ASSERT
*/
#define MSG_METHOD_LOGGING(...) ::GuelderEngine::Debug::Logger::Format(GGetClassName(), "::", FUNC_NAME, ": ", __VA_ARGS__)

/**
* @brief guelder engine error log
* @param all debug info(it can be chars, ints, floats, and other types which support '<<' operator)
*/
#define GE_THROW(...) ::GuelderEngine::Debug::Logger::Throw(Debug::Logger::Format(__VA_ARGS__), __FILE__, __LINE__)

/**
* @brief This macro is used in core classes which are inherited from GuelderEngine::Core::GClass.
* If condition(param) is false then it will throw exception. Prints the path of the file and msg(param).
*
* @param condition - bool
* @param ... - message, any type that supports "<<" operator
*/
#define GE_CORE_CLASS_ASSERT(condition, ...) ::GuelderEngine::Debug::Logger::Assert(condition, MSG_METHOD_LOGGING(__VA_ARGS__), __FILE__, __LINE__)

#define GE_CORE_CLASS_THROW(...) ::GuelderEngine::Debug::Logger::Throw(MSG_METHOD_LOGGING(__VA_ARGS__), __FILE__, __LINE__)

/**
* @brief If condition(param) is false then it will throw exception. Prints the path of the file and message(msg).
*
* @param condition - bool
* @param ... - message, any type that supports "<<" operator
*/
#define GE_ASSERT(condition, ...) ::GuelderEngine::Debug::Logger::Assert(condition, ::GuelderEngine::Debug::Logger::Format(__VA_ARGS__), __FILE__, __LINE__)

#define GE_ASSERT_FN(condition, ...) GE_ASSERT(condition, FUNC_NAME, ": ", __VA_ARGS__)

#define GE_TO_STRING(arg) #arg
#define CONCATENATE(lhs, rhs) GE_TO_STRING(lhs ## rhs)

#define LOG_CATEGORY_VARIABLE(name) name##LoggingCategoryVar
#define LOG_CATEGORY(name) name##LoggingCategory

#define DECLARE_LOG_CATEGORY_EXTERN(name, loggingLevels, enable)\
    struct LOG_CATEGORY(name) final : ::GuelderEngine::Debug::LoggingCategory<::GuelderEngine::Debug::LogLevel::##loggingLevels, enable>\
    {\
    LOG_CATEGORY(name)() : ::GuelderEngine::Debug::LoggingCategory<::GuelderEngine::Debug::LogLevel::##loggingLevels, enable>(GE_TO_STRING(name)) {}\
    };\
    extern LOG_CATEGORY(name) LOG_CATEGORY_VARIABLE(name)


#ifdef GE_DEBUG

#define DEFINE_LOG_CATEGORY(name) LOG_CATEGORY(name) LOG_CATEGORY_VARIABLE(name)

#define GE_LOG(categoryName, level, ...) ::GuelderEngine::Debug::Log<LOG_CATEGORY(categoryName)::supportedLoggingLevels, LOG_CATEGORY(categoryName)::enable>(LOG_CATEGORY_VARIABLE(categoryName), ::GuelderEngine::Debug::LogLevel::##level, __VA_ARGS__)

#else
#define DEFINE_LOG_CATEGORY(...)
#define GE_LOG(...)
#endif