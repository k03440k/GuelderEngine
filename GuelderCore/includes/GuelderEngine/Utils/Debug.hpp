#pragma once

import GuelderEngine.Debug;

#define GE_DEBUG

#ifdef GE_DEBUG

#define GE_DEBUG_VULKAN

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

#define GE_TO_STRING(arg) #arg
#define CONCATENATE(lhs, rhs) GE_TO_STRING(lhs ## rhs)


#define DECLARE_LOG_CATEGORY_EXTERN(name, loggingLevels)\
    struct name##LoggingCategory final : ::GuelderEngine::Debug::LoggingCategory<::GuelderEngine::Debug::LogLevel::##loggingLevels>\
    {\
    name##LoggingCategory() : ::GuelderEngine::Debug::LoggingCategory<::GuelderEngine::Debug::LogLevel::##loggingLevels>(#name) {}\
    };\
    extern name##LoggingCategory name##LoggingCategoryVar

#ifdef GE_DEBUG

#define DEFINE_LOG_CATEGORY(name) name##LoggingCategory name##LoggingCategoryVar

#define GE_LOG(categoryName, level, ...) ::GuelderEngine::Debug::Log(categoryName##LoggingCategoryVar, ::GuelderEngine::Debug::LogLevel::##level, __VA_ARGS__)

#else
#define DEFINE_LOG_CATEGORY(...)
#define GE_LOG(...)
#endif