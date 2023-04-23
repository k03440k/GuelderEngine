#GuelderEngineEditor - EDITOR
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(ENGINE_EDITOR_NAME "GuelderEngineEditor")

set(CMAKE_CXX_STANDARD 20)

project (${ENGINE_EDITOR_NAME})

#add sources
set(EDITOR_SOURCE_PATH "src/")

add_executable(${ENGINE_EDITOR_NAME}
	${EDITOR_SOURCE_PATH}main.cpp
)

target_link_libraries(${ENGINE_EDITOR_NAME} PRIVATE GuelderEngineCore)
set_target_properties(${ENGINE_EDITOR_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)