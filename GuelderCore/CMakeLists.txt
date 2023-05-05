#GuelderEngineCore - CORE
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(ENGINE_CORE_NAME "GuelderEngineCore")

set(CMAKE_CXX_STANDARD 20)

project (${ENGINE_CORE_NAME})

set(CORE_PRIVATE_HEADERS_PATH src/headers/)
set(CORE_PRIVATE_SOURCES_PATH src/sources/)
set(CORE_PUBLIC_INCLUDES_PATH includes/GuelderEngine/)

set(CORE_PRIVATE_SOURCES
	${CORE_PRIVATE_SOURCES_PATH}Utils/Debug.cpp
	${CORE_PRIVATE_SOURCES_PATH}Utils/ResourceManager.cpp
	
	${CORE_PRIVATE_SOURCES_PATH}Application.cpp
	${CORE_PRIVATE_SOURCES_PATH}Window.cpp
	${CORE_PRIVATE_SOURCES_PATH}Layers/LayerStack.cpp
)
set(CORE_PRIVATE_HEADERS
	${CORE_PRIVATE_HEADERS_PATH}Window.hpp
)
set(CORE_PUBLIC_HEADERS
	${CORE_PUBLIC_INCLUDES_PATH}GuelderEngine.hpp
	${CORE_PUBLIC_INCLUDES_PATH}Application.hpp
	
	${CORE_PUBLIC_INCLUDES_PATH}Utils/Utils.hpp
	${CORE_PUBLIC_INCLUDES_PATH}Utils/Debug.hpp
	${CORE_PUBLIC_INCLUDES_PATH}Utils/ResourceManager.hpp

	${CORE_PUBLIC_INCLUDES_PATH}Events/Event.hpp
	${CORE_PUBLIC_INCLUDES_PATH}Events/MouseEvent.hpp
	${CORE_PUBLIC_INCLUDES_PATH}Events/KeyEvent.hpp
	${CORE_PUBLIC_INCLUDES_PATH}Events/ApplicationEvent.hpp
	
	${CORE_PUBLIC_INCLUDES_PATH}Layers/Layer.hpp
	${CORE_PUBLIC_INCLUDES_PATH}Layers/LayerStack.hpp
)
set(CORE_SOURCE
	${CORE_PRIVATE_SOURCES}
	${CORE_PRIVATE_HEADERS}
	${CORE_PUBLIC_HEADERS}
)

add_library(${ENGINE_CORE_NAME} STATIC
	${CORE_SOURCE}
	../External/stb_image.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${CORE_SOURCE}
)

target_include_directories(${ENGINE_CORE_NAME} PUBLIC includes)
target_include_directories(${ENGINE_CORE_NAME} PRIVATE src)

#include external libs

#start include glfw
#GLFW (maybe temp)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

option(GLFW_USE_STATIC_VULKAN_LOADER "Use the static Vulkan loader" ON)

add_subdirectory(../External/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_CORE_NAME} PUBLIC glfw)
#end include glfw

#start inlude glm
add_subdirectory(../External/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_CORE_NAME} PUBLIC glm)
#end include glm

#start ImGui include
set(IMGUI_INCLUDES 
	../External/imgui/imgui.h
	../External/imgui/backends/imgui_impl_glfw.h
	#../External/imgui/backends/imgui_impl_opengl3.h
	)
	
set(IMGUI_SOURCES
	../External/imgui/imgui.cpp
	../External/imgui/imgui_draw.cpp
	../External/imgui/imgui_tables.cpp
	../External/imgui/imgui_widgets.cpp
	../External/imgui/imgui_demo.cpp
	../External/imgui/backends/imgui_impl_glfw.cpp
	#../External/imgui/backends/imgui_impl_opengl3.cpp
	)
	
add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
	)
	
target_include_directories(ImGui PUBLIC ../External)
target_include_directories(ImGui PUBLIC ../External/imgui)

target_link_libraries(ImGui PRIVATE glfw)
target_link_libraries(${ENGINE_CORE_NAME} PRIVATE ImGui)
#end ImGui include

#start Vulkan include
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()
target_link_libraries(${ENGINE_CORE_NAME} PUBLIC Vulkan::Vulkan)

if(GLFW_USE_STATIC_VULKAN_LOADER)
    target_compile_definitions(${ENGINE_CORE_NAME} PRIVATE GLFW_INCLUDE_NONE GLFW_USE_STATIC_VULKAN_LOADER)
else()
    target_compile_definitions(${ENGINE_CORE_NAME} PRIVATE GLFW_INCLUDE_VULKAN)
endif()
#end Vulkan include

#start Vulkan-HPP include
#add_subdirectory(../External/vulkanhpp ${CMAKE_CURRENT_BINARY_DIR}/vulkanhpp)
#target_link_libraries(${ENGINE_CORE_NAME} PUBLIC vulkanhpp)
#end Vulkan-HPP include

#etc.
set_target_properties(${ENGINE_CORE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${ENGINE_CORE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:${ENGINE_CORE_NAME}>/Resources)