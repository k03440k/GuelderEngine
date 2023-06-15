#GuelderEngineCore - CORE
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(ECN "GuelderEngineCore")#ENGINE_CORE_NAME

set(CMAKE_CXX_STANDARD 20)#TODO: GO TO FUCKING MODULES!!!!!!!!!!!!!
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)
set(CXX_SCAN_FOR_MODULES ON)

project (${ECN})

set(CORE_PRIVATE_MODULES_PATH src/headers/)
set(CORE_PRIVATE_SOURCES_PATH src/sources/)
set(CORE_PUBLIC_MODULES_PATH includes/GuelderEngine/)

set(CORE_PRIVATE_HEADERS

)
set(CORE_PUBLIC_HEADERS
	${CORE_PUBLIC_MODULES_PATH}Utils/Debug.hpp
	${CORE_PRIVATE_MODULES_PATH}Events/Event.hpp
)
set(CORE_PRIVATE_SOURCES
	#${CORE_PRIVATE_SOURCES_PATH}GObject/GClass.cpp

	${CORE_PRIVATE_SOURCES_PATH}Utils/Debug.cpp
	${CORE_PRIVATE_SOURCES_PATH}Utils/ResourceManager.cpp
	
	${CORE_PRIVATE_SOURCES_PATH}Core/Application.cpp
	${CORE_PRIVATE_SOURCES_PATH}Core/Window.cpp
	${CORE_PRIVATE_SOURCES_PATH}Layers/LayerStack.cpp
	
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/VulkanManager.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/VulkanDebugManager.cpp
)
set(CORE_PRIVATE_MODULES
	${CORE_PRIVATE_MODULES_PATH}Core/Window.ixx
	
	${CORE_PRIVATE_MODULES_PATH}Core/GObject/GClass.ixx
	
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Vulkan.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/VulkanManager.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/VulkanDebugManager.ixx
)
set(CORE_PUBLIC_MODULES
	${CORE_PUBLIC_MODULES_PATH}GuelderEngine.ixx
	
	${CORE_PUBLIC_MODULES_PATH}Core/Core.ixx
	${CORE_PUBLIC_MODULES_PATH}Core/Application.ixx
	
	${CORE_PUBLIC_MODULES_PATH}Core/Types.ixx
	${CORE_PUBLIC_MODULES_PATH}Utils/Debug.ixx
	${CORE_PUBLIC_MODULES_PATH}Utils/ResourceManager.ixx

	${CORE_PUBLIC_MODULES_PATH}Events/Events.ixx
	${CORE_PUBLIC_MODULES_PATH}Events/BaseEvent.ixx
	${CORE_PUBLIC_MODULES_PATH}Events/MouseEvent.ixx
	${CORE_PUBLIC_MODULES_PATH}Events/KeyEvent.ixx
	${CORE_PUBLIC_MODULES_PATH}Events/ApplicationEvent.ixx
	
	${CORE_PUBLIC_MODULES_PATH}Layers/Layers.ixx
	${CORE_PUBLIC_MODULES_PATH}Layers/Layer.ixx
	${CORE_PUBLIC_MODULES_PATH}Layers/LayerStack.ixx
)
set(CORE_MODULES_SOURCE
	${CORE_PRIVATE_MODULES}
	${CORE_PRIVATE_SOURCES}
	${CORE_PUBLIC_MODULES}
)
set(CORE_SOURCE
	${CORE_MODULES_SOURCE}
	${CORE_PRIVATE_HEADERS}
	${CORE_PUBLIC_HEADERS}
)

add_library(${ECN} STATIC
	${CORE_SOURCE}
	../External/stb_image.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${CORE_SOURCE}
)
target_sources(${ECN}
        PRIVATE
			FILE_SET module_files_public TYPE CXX_MODULES
			FILES
			${CORE_PUBLIC_MODULES}
)
target_sources(
        ${ECN}
		PRIVATE
        FILE_SET module_files_private TYPE CXX_MODULES
        FILES
		#PRIVATE
		${CORE_PRIVATE_MODULES}
		${CORE_PRIVATE_SOURCES}
)
#target_sources(
#        ${ECN}
#        PUBLIC
#        FILE_SET header_files TYPE HEADERS
#        BASE_DIRS "../GuelderCore"
#        FILES
#${CORE_PUBLIC_MODULES}
#		${CORE_PRIVATE_HEADERS}
#		${CORE_PUBLIC_HEADERS}
#)

target_include_directories(${ECN} PUBLIC includes)
target_include_directories(${ECN} PRIVATE src)

#include external libs

#start include glfw
#GLFW (maybe temp)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

option(GLFW_USE_STATIC_VULKAN_LOADER "Use the static Vulkan loader" ON)

add_subdirectory(../External/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ECN} PRIVATE glfw)
#end include glfw

#start inlude glm
add_subdirectory(../External/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ECN} PRIVATE glm)
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
	
#target_include_directories(ImGui PRIVATE ../External)
target_include_directories(ImGui PRIVATE ../External/imgui)
target_include_directories(${ECN} PRIVATE ../External)#???

target_link_libraries(ImGui PRIVATE glfw)
target_link_libraries(${ECN} PRIVATE ImGui)
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
target_link_libraries(${ECN} PUBLIC Vulkan::Vulkan)

if(GLFW_USE_STATIC_VULKAN_LOADER)
    target_compile_definitions(${ECN} PRIVATE GLFW_INCLUDE_NONE GLFW_USE_STATIC_VULKAN_LOADER)
else()
    target_compile_definitions(${ECN} PRIVATE GLFW_INCLUDE_VULKAN)
endif()
#end Vulkan include

#start Vulkan-hpp include
#add_subdirectory(../External/vulkanhpp ${CMAKE_CURRENT_BINARY_DIR}/vulkanhpp)
#target_link_libraries(${ECN} PUBLIC vulkanhpp)
#end Vulkan-hpp include

#etc.
set_target_properties(${ECN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${ECN} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:${ECN}>/Resources)