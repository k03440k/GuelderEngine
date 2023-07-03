#pragma once

import GuelderEngine.Core.Types;

#define INHERIT_GClass(className) public virtual ::GuelderEngine::Core::GClass<className>

#define DELETE_COPY_AND_MOVE(className)\
    className(const className &) = delete;\
    className(className&&) = delete;\
    className& operator=(const className&) = delete;\
    className& operator=(className&&) = delete