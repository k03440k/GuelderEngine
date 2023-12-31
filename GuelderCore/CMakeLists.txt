#GuelderEngineCore - CORE
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(ECN "GuelderEngineCore")#ENGINE_CORE_NAME

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)
set(CXX_SCAN_FOR_MODULES ON)

project (${ECN})

set(CORE_PRIVATE_MODULES_PATH private/headers/)
set(CORE_PRIVATE_SOURCES_PATH private/sources/)
set(CORE_PUBLIC_MODULES_PATH public/GuelderEngine/)

set(CORE_PRIVATE_HEADERS
	${CORE_PRIVATE_MODULES_PATH}Core/GObject/GClass.hpp
	${CORE_PRIVATE_MODULES_PATH}Events/Event.hpp
)
set(CORE_PUBLIC_HEADERS
	${CORE_PUBLIC_MODULES_PATH}Utils/Debug.hpp
	${CORE_PUBLIC_MODULES_PATH}GuelderEngine.hpp
)
set(CORE_PRIVATE_SOURCES
	#${CORE_PRIVATE_SOURCES_PATH}GObject/GClass.cpp
	
	${CORE_PRIVATE_SOURCES_PATH}Utils/Debug.cpp
	${CORE_PRIVATE_SOURCES_PATH}Utils/ResourceManager.cpp
	
	${CORE_PRIVATE_SOURCES_PATH}Core/Application.cpp
	${CORE_PRIVATE_SOURCES_PATH}Core/Window.cpp
	${CORE_PRIVATE_SOURCES_PATH}Layers/LayerStack.cpp
	
	${CORE_PRIVATE_SOURCES_PATH}Objects/World.cpp
	${CORE_PRIVATE_SOURCES_PATH}Objects/ObjectBase.cpp
	${CORE_PRIVATE_SOURCES_PATH}Objects/Object2D.cpp
	${CORE_PRIVATE_SOURCES_PATH}Objects/Object3D.cpp
	${CORE_PRIVATE_SOURCES_PATH}Objects/Actor.cpp
	
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/Renderer.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/RenderSystem.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/Manager.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/DebugManager.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/DeviceManager.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/Swapchain.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/ShaderManager.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/Pipeline.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/SwapchainFrameSync.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/QueueFamilyIndices.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/CommandPool.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/FrameBuffer.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/SwapchainFrame.cpp
	#${CORE_PRIVATE_SOURCES_PATH}Vulkan/Mesh.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/Buffers/IBuffer.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/Buffers/VertexBuffer.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/Buffers/StagingBuffer.cpp
	${CORE_PRIVATE_SOURCES_PATH}Vulkan/Buffers/IndexBuffer.cpp
)
set(CORE_PRIVATE_MODULES
	${CORE_PRIVATE_MODULES_PATH}Core/Window.ixx
	
	#${CORE_PRIVATE_MODULES_PATH}Vulkan/Vulkan.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Manager.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/DebugManager.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/DeviceManager.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/IVulkanObject.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Swapchain.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/ShaderManager.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Pipeline.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/SwapchainFrameSync.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/QueueFamilyIndices.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/CommandPool.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/FrameBuffer.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/SwapchainFrame.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Buffers/IBuffer.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Buffers/VertexBuffer.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Buffers/StagingBuffer.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Buffers/IndexBuffer.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/Renderer.ixx
	${CORE_PRIVATE_MODULES_PATH}Vulkan/RenderSystem.ixx
)
set(CORE_PUBLIC_MODULES
	${CORE_PUBLIC_MODULES_PATH}GuelderEngine.ixx
	
	${CORE_PUBLIC_MODULES_PATH}Vulkan/Mesh.ixx
	${CORE_PUBLIC_MODULES_PATH}Vulkan/Vulkan.ixx
	
	${CORE_PUBLIC_MODULES_PATH}Core/Core.ixx
	${CORE_PUBLIC_MODULES_PATH}Core/Application.ixx
	
	${CORE_PUBLIC_MODULES_PATH}Objects/Game.ixx
	${CORE_PUBLIC_MODULES_PATH}Objects/World.ixx
	${CORE_PUBLIC_MODULES_PATH}Objects/ObjectBase.ixx
	${CORE_PUBLIC_MODULES_PATH}Objects/Object2D.ixx
	${CORE_PUBLIC_MODULES_PATH}Objects/Object3D.ixx
	${CORE_PUBLIC_MODULES_PATH}Objects/Actor.ixx
	
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
	${CORE_PUBLIC_MODULES}
	${CORE_PRIVATE_MODULES}
	${CORE_PRIVATE_SOURCES}
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
        PUBLIC
		FILE_SET module_files_public TYPE CXX_MODULES
		FILES
		
		${CORE_PUBLIC_MODULES}
)
target_sources(
        ${ECN}
		PRIVATE
        FILE_SET module_files_private TYPE CXX_MODULES
        FILES

		${CORE_PRIVATE_MODULES}
		${CORE_PRIVATE_SOURCES}
)

target_include_directories(${ECN} PUBLIC public)
target_include_directories(${ECN} PRIVATE private)

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
  find_package(Vulkan REQUIRED COMPONENTS glslc) # throws error if could not find Vulkan
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

function(ge_copy_directory OUTPUT_DIRECTORY)
add_custom_command(TARGET ${ECN} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources ${OUTPUT_DIRECTORY})
endfunction()

ge_copy_directory(${CMAKE_BINARY_DIR}/bin/Debug/Resources)
ge_copy_directory(${CMAKE_BINARY_DIR}/bin/Release/Resources)
ge_copy_directory($<TARGET_FILE_DIR:${ECN}>/Resources)

#start compile .glsl shaders

if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
  set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
  set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/*.vert"
    "${CMAKE_SOURCE_DIR}/Resources/Shaders/*.frag"
    )

find_program(GLSLC_EXECUTABLE NAMES glslc HINTS Vulkan::glslc)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(DSPIRV "${CMAKE_BINARY_DIR}/bin/Debug/Resources/Shaders/Compiled/${FILE_NAME}.spv")
  set(RSPIRV "${CMAKE_BINARY_DIR}/bin/Release/Resources/Shaders/Compiled/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${DSPIRV}
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/Debug/Resources/Shaders/Compiled/"
	COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${DSPIRV}
    COMMAND ${GLSLC_EXECUTABLE} -o ${DSPIRV} ${GLSL}
    DEPENDS ${GLSL} "${CMAKE_BINARY_DIR}/bin/Debug/Resources/Shaders/Compiled/")
	add_custom_command(
    OUTPUT ${RSPIRV}
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/Release/Resources/Shaders/Compiled/"
	COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${RSPIRV}
    COMMAND ${GLSLC_EXECUTABLE} -o ${RSPIRV} ${GLSL}
    DEPENDS ${GLSL} "${CMAKE_BINARY_DIR}/bin/Release/Resources/Shaders/Compiled/")
  list(APPEND SPIRV_BINARY_FILES ${DSPIRV})
  list(APPEND SPIRV_BINARY_FILES ${RSPIRV})
endforeach(GLSL)

add_custom_target(Shaders ALL DEPENDS ${SPIRV_BINARY_FILES})

add_dependencies(${ECN} Shaders)

add_custom_command(TARGET ${ECN} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${ECN}>/Resources/Shaders/Compiled/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/bin/Release/Resources/Shaders/Compiled/"
        $<TARGET_FILE_DIR:${ECN}>/Resources/Shaders/Compiled/
        )
#end compile .glsl shaders