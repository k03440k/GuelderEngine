#GuelderEngineEditor - EDITOR
cmake_minimum_required (VERSION 3.26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(ENGINE_EDITOR_NAME "GuelderEngineEditor")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)
set(CXX_SCAN_FOR_MODULES ON)

project (${ENGINE_EDITOR_NAME})

#add sources
set(EDITOR_SOURCE_PATH "src/")
add_executable(${ENGINE_EDITOR_NAME}
${EDITOR_SOURCE_PATH}main.cpp
)
target_sources(
        ${ENGINE_EDITOR_NAME}
        PUBLIC
		${EDITOR_SOURCE_PATH}main.cpp
		)

target_link_libraries(${ENGINE_EDITOR_NAME} PUBLIC GuelderEngineCore)
set_target_properties(${ENGINE_EDITOR_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)