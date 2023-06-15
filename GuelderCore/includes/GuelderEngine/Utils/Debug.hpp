#pragma once

import GuelderEngine.Debug;

#define DEBUG_VULKAN
#define GE_DEBUG

#define FUNC_NAME __func__

/*
* @brief Use this macro only in classes which inherited from GuelderEngine::Core::GClass
*/
#define METHOD_NAME ::GuelderEngine::Debug::Logger::Format(GGetClassName(), "::", FUNC_NAME)

/*
* @brief Use this macro only for GE_CORE_ASSERT
*
* @example
*/
#define MSG_METHOD_LOGGING(msg) ::GuelderEngine::Debug::Logger::Format(GGetClassName(), "::", FUNC_NAME, ": ", msg)

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