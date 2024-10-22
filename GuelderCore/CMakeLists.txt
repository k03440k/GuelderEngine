#GuelderEngineCore - CORE
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)
set(CXX_SCAN_FOR_MODULES ON)

set(ENGINE_CORE_NAME "GuelderEngineCore")

project(${ENGINE_CORE_NAME})

set(CORE_PRIVATE_MODULES_PATH private/headers)
set(CORE_PRIVATE_SOURCES_PATH private/sources)
set(CORE_PUBLIC_MODULES_PATH public/GuelderEngine)

set(CORE_PRIVATE_HEADERS
	"${CORE_PRIVATE_MODULES_PATH}/Core/GObject/GClass.hpp"
	"${CORE_PRIVATE_MODULES_PATH}/Events/Event.hpp"
)
set(CORE_PUBLIC_HEADERS
	"${CORE_PUBLIC_MODULES_PATH}/Utils/Debug.hpp"
	"${CORE_PUBLIC_MODULES_PATH}/GuelderEngine.hpp"
)
set(CORE_PRIVATE_SOURCES
	"${CORE_PRIVATE_SOURCES_PATH}/Utils/Debug.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Utils/ResourceManager.cpp"
	
	"${CORE_PRIVATE_SOURCES_PATH}/Core/Application.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Core/Window.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Layers/LayerStack.cpp"
	
	"${CORE_PRIVATE_SOURCES_PATH}/Actors/World.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Actors/GObject.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Actors/PlayerController.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Actors/GameMode.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Actors/KeyboardController.cpp"
	
	"${CORE_PRIVATE_SOURCES_PATH}/ActorComponents/CameraComponent.cpp"
	
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Renderer.cpp"
	#"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/RenderSystem.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Manager.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/DebugManager.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/DeviceManager.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Swapchain.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/ShaderManager.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/FrameSync.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/QueueFamilyIndices.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/CommandPool.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/FrameContext.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/SwapchainFrame.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/SwapchainDepthImage.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Buffers/Buffer.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Buffers/VertexBuffer.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Buffers/StagingBuffer.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Buffers/IndexBuffer.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Surface.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Descriptors/DescriptorPool.cpp"
	"${CORE_PRIVATE_SOURCES_PATH}/Vulkan/Descriptors/DescriptorSetLayout.cpp"
	
	"${CORE_PRIVATE_SOURCES_PATH}/UserInterface/UserInterfaceManager.cpp"
)
set(CORE_PRIVATE_MODULES
	"${CORE_PRIVATE_MODULES_PATH}/Core/Window.ixx"
	
	#"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Vulkan.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Manager.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/DebugManager.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/DeviceManager.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/IVulkanObject.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Swapchain.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/ShaderManager.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Pipeline.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/FrameSync.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/QueueFamilyIndices.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/CommandPool.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/FrameContext.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/SwapchainFrame.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/SwapchainDepthImage.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Buffers/Buffer.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Buffers/VertexBuffer.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Buffers/StagingBuffer.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Buffers/IndexBuffer.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Renderer.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/RenderSystem.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Surface.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Descriptors/DescriptorPool.ixx"
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/Descriptors/DescriptorSetLayout.ixx"
	
	"${CORE_PRIVATE_MODULES_PATH}/Vulkan/MeshAllocator.ixx"
)
set(CORE_PUBLIC_MODULES
	"${CORE_PUBLIC_MODULES_PATH}/GuelderEngine.ixx"
	
	"${CORE_PUBLIC_MODULES_PATH}/Vulkan/Mesh.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Vulkan/Vulkan.ixx"
	
	"${CORE_PUBLIC_MODULES_PATH}/Core/Core.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Core/Utils.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Core/Application.ixx"
	
	"${CORE_PUBLIC_MODULES_PATH}/Actors/Actors.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Actors/PlayerController.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Actors/GameMode.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Actors/World.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Actors/GObject.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Actors/Actor2D.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Actors/Actor3D.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Actors/Actor.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Actors/KeyboardController.ixx"
	
	"${CORE_PUBLIC_MODULES_PATH}/ActorComponents/ActorComponent.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/ActorComponents/CameraComponent.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/ActorComponents/MeshComponent.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/ActorComponents/RenderActorTransform.ixx"
	
	"${CORE_PUBLIC_MODULES_PATH}/Core/Types.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Utils/Debug.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Utils/ResourceManager.ixx"

	"${CORE_PUBLIC_MODULES_PATH}/Events/Events.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Events/BaseEvent.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Events/MouseEvent.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Events/KeyEvent.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Events/ApplicationEvent.ixx"
	
	"${CORE_PUBLIC_MODULES_PATH}/Layers/Layers.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Layers/Layer.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/Layers/LayerStack.ixx"
	
	"${CORE_PUBLIC_MODULES_PATH}/UserInterface/UserInterface.ixx"
	"${CORE_PUBLIC_MODULES_PATH}/UserInterface/UserInterfaceManager.ixx"
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

add_library(${ENGINE_CORE_NAME} STATIC
	${CORE_SOURCE}
	"../External/stb_image.h"
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${CORE_SOURCE}
)
target_sources(${ENGINE_CORE_NAME}
        PUBLIC
		FILE_SET module_files_public TYPE CXX_MODULES
		FILES
		
		${CORE_PUBLIC_MODULES}
)
target_sources(
        ${ENGINE_CORE_NAME}
		PRIVATE
        FILE_SET module_files_private TYPE CXX_MODULES
        FILES

		${CORE_PRIVATE_MODULES}
		${CORE_PRIVATE_SOURCES}
)

target_include_directories(${ENGINE_CORE_NAME} PUBLIC public)
target_include_directories(${ENGINE_CORE_NAME} PRIVATE private)

#include external libs

#start include glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory("../External/glfw" "${CMAKE_CURRENT_BINARY_DIR}/glfw")
target_link_libraries(${ENGINE_CORE_NAME} PRIVATE glfw)
#end include glfw

#start inlude glm
add_subdirectory("../External/glm" "${CMAKE_CURRENT_BINARY_DIR}/glm")
target_link_libraries(${ENGINE_CORE_NAME} PUBLIC glm)
#end include glm

#start Vulkan include
option(GLFW_USE_STATIC_VULKAN_LOADER "Use the static Vulkan loader" ON)

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
target_link_libraries(${ENGINE_CORE_NAME} PUBLIC Vulkan::Vulkan)

if(GLFW_USE_STATIC_VULKAN_LOADER)
    target_compile_definitions(${ENGINE_CORE_NAME} PRIVATE GLFW_INCLUDE_NONE GLFW_USE_STATIC_VULKAN_LOADER)
else()
    target_compile_definitions(${ENGINE_CORE_NAME} PRIVATE GLFW_INCLUDE_VULKAN)
endif()
#end Vulkan include

#start ImGui include
set(IMGUI_INCLUDES 
	"../External/imgui/imgui.h"
	"../External/imgui/imconfig.h"
	"../External/imgui/imgui_internal.h"
	"../External/imgui/imstb_rectpack.h"
	"../External/imgui/imstb_textedit.h"
	"../External/imgui/imstb_truetype.h"
	"../External/imgui/backends/imgui_impl_glfw.h"
	"../External/imgui/backends/imgui_impl_vulkan.h"
	)
	
set(IMGUI_SOURCES
	"../External/imgui/imgui.cpp"
	"../External/imgui/imgui_draw.cpp"
	"../External/imgui/imgui_tables.cpp"
	"../External/imgui/imgui_widgets.cpp"
	"../External/imgui/imgui_demo.cpp"
	"../External/imgui/backends/imgui_impl_glfw.cpp"
	"../External/imgui/backends/imgui_impl_vulkan.cpp"
	)
set(IMGUI_MICS
	"../External/imgui/misc/debuggers/imgui.natvis"
	"../External/imgui/misc/debuggers/imgui.natstepfilter"
	)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
	${IMGUI_MICS}
	)
	
target_include_directories(ImGui PUBLIC "../External/imgui")
target_include_directories(${ENGINE_CORE_NAME} PUBLIC "../External")#???

target_link_libraries(ImGui PUBLIC Vulkan::Vulkan)
target_link_libraries(ImGui PUBLIC glfw)
target_link_libraries(${ENGINE_CORE_NAME} PUBLIC ImGui)
#end ImGui include

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
target_link_libraries(${ENGINE_CORE_NAME} PUBLIC GuelderEngineEditor)

foreach(SHADER ${GLSL_SOURCE_FILES})

    get_filename_component(FILE_NAME ${SHADER} NAME)
	set(SHADER_BIN "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Resources/Shaders/Compiled/${FILE_NAME}.spv")#shader binary

    add_custom_command(
		OUTPUT ${SHADER_BIN}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Resources/Shaders/Compiled/"
		COMMAND ${GLSL_VALIDATOR} -V ${SHADER} -o ${SHADER_BIN}
        DEPENDS ${SHADER} "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Resources/Shaders/Compiled/"
		MESSAGE "Outputting ${FILE_NAME} into ${SHADER_BIN}"
    )

list(APPEND SPIRV_BINARY_FILES ${SHADER_BIN})

endForeach(SHADER)

add_custom_target(Shaders ALL DEPENDS ${SPIRV_BINARY_FILES})
add_dependencies(${ENGINE_CORE_NAME} Shaders)
#end compile .glsl shaders

#copying Resources directory
add_custom_command(TARGET ${ENGINE_CORE_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/Resources"
	"${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Resources"
	)

#etc
set_target_properties(${ENGINE_CORE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")