#pragma once

import GuelderEngine.Core.Types;

#define INHERIT_GClass(className) /*public virtual ::GuelderEngine::GClass<className>*/

#define GENERATE_DEBUG(className)\
    const char* GGetClassName() override { return typeid(*this).name(); }\
    const char* GGetClassRawName() override { return typeid(*this).raw_name(); }

#define DELETE_COPY(className)\
    className(const className&) = delete;\
    className& operator=(const className&) = delete

#define DELETE_MOVING(className)\
    className(className&&) = delete;\
    className& operator=(className&&) = delete

#define DELETE_COPY_AND_MOVING(className)\
    DELETE_COPY(className);\
    DELETE_MOVING(className)

#define NO_INSTANCE_CLASS(className)\
    className() = delete;\
    ~className() = delete;\
    DELETE_COPY_AND_MOVING(className)

/**
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

/**
 *@brief declare default ctor and dtor and copying and moving
*/
#define DECLARE_DCAD_AND_CAM(className)\
    DECLARE_DEFAULT_CTOR_AND_DTOR(className);\
    DECLARE_COPY_AND_MOVING(className)

#define DEFINE_INTERFACE(interfaceName)\
    virtual ~interfaceName() = default

#define DECLARE_DEFAULT_COPY(className)\
    className(const className&) = default;\
    className& operator=(const className&) = default

#define DECLARE_DEFAULT_MOVING(className)\
    className(className&&) noexcept = default;\
    className& operator=(className&&) noexcept = default

/**
 *@brief declares ctor, dtor, assignment operators as default
*/
#define DECLARE_DEFAULT(className)\
    DECLARE_DEFAULT_CTOR_AND_DTOR(className);\
    DECLARE_DEFAULT_COPY(className);\
    DECLARE_DEFAULT_MOVING(className)