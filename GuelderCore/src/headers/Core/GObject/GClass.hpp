#pragma once

import GuelderEngine.Core.Types;

#define INHERIT_GClass(className) public virtual ::GuelderEngine::Core::GClass<className>

#define DELETE_COPY(className)\
    className(const className &) = delete;\
    className& operator=(const className&) = delete

#define DELETE_MOVING(className)\
    className(className&&) = delete;\
    className& operator=(className&&) = delete

#define DELETE_COPY_AND_MOVING(className)\
    DELETE_COPY(className);\
    DELETE_MOVING(className)

/*
 *@brief declares copy operator and ctor without "default" keyword
*/
#define DECLARE_COPY(className)\
    className(const className& other);\
    className& operator=(const className& other)

#define DECLARE_MOVING(className)\
    className(className&& other) noexcept;\
    className& operator=(className&& other) noexcept

#define DECLARE_COPY_AND_MOVING(className)\
    DECLARE_COPY(className);\
    DECLARE_MOVING(className)

#define DECLARE_DEFAULT_CTOR_AND_DTOR(className)\
    className() = default;\
    ~className() = default

/*
 *@brief declare default ctor and dtor and copying and moving
*/
#define DECLARE_DCAD_AND_CAM(className)\
    DECLARE_DEFAULT_CTOR_AND_DTOR(className);\
    DECLARE_COPY_AND_MOVING(className)