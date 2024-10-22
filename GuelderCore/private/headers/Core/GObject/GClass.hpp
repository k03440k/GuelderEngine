#pragma once

//deleting

#define DELETE_COPYING(className)\
    className(const className&) = delete;\
    className& operator=(const className&) = delete

#define DELETE_MOVING(className)\
    className(className&&) = delete;\
    className& operator=(className&&) = delete

#define DELETE_COPYING_AND_MOVING(className)\
    DELETE_COPYING(className);\
    DELETE_MOVING(className)

#define DELETE_CTOR_AND_DTOR(className)\
    className() = delete;\
    ~className() = delete

/// <summary>
/// Declares ctor, dtor, assignment operators as deleted
/// </summary>
#define NO_INSTANCE(className)\
    DELETE_CTOR_AND_DTOR(className);\
    DELETE_COPYING_AND_MOVING(className)

//declarations

#define DECLARE_COPYING(className)\
    className(const className& other);\
    className& operator=(const className& other)

#define DECLARE_MOVING(className)\
    className(className&& other) noexcept;\
    className& operator=(className&& other) noexcept

#define DECLARE_COPYING_AND_MOVING(className)\
    DECLARE_COPYING(className);\
    DECLARE_MOVING(className)

/// <summary>
/// Declares dtor as virtual
/// </summary>
#define DECLARE_INTERFACE(interfaceName)\
    virtual ~interfaceName() = default

//default declarations

#define DECLARE_DEFAULT_CTOR_AND_DTOR(className)\
    className() = default;\
    ~className() = default

#define DECLARE_DEFAULT_COPYING(className)\
    className(const className&) = default;\
    className& operator=(const className&) = default

#define DECLARE_DEFAULT_MOVING(className)\
    className(className&&) noexcept = default;\
    className& operator=(className&&) noexcept = default

#define DECLARE_DEFAULT_COPYING_AND_MOVING(className)\
    DECLARE_DEFAULT_COPYING(className);\
    DECLARE_DEFAULT_MOVING(className)

/// <summary>
/// Declares ctor, dtor, assignment operators as default
/// </summary>
#define DECLARE_DEFAULT(className)\
    DECLARE_DEFAULT_CTOR_AND_DTOR(className);\
    DECLARE_DEFAULT_COPYING(className);\
    DECLARE_DEFAULT_MOVING(className)

//etc.

#define FORCE_INLINE __forceinline