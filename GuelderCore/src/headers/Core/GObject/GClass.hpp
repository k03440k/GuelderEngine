#pragma once

import GuelderEngine.Core.Types;

#define INHERIT_GClass(className) public virtual ::GuelderEngine::Core::GClass<className>

#define DELETE_COPY(className)\
    className(const className &) = delete;\
    className& operator=(const className&) = delete;\

#define DELETE_MOVING(className)\
    className(className&&) = delete;\
    className& operator=(className&&) = delete

#define DELETE_COPY_AND_MOVING(className)\
    DELETE_COPY(className);\
    DELETE_MOVING(className)