# (c) 2021-2023 Eric Fedosejevs and contributors. Licensed under MIT license (see LICENSE for details)

cmake_minimum_required(VERSION 3.25)

# Set the project name and version
#project(UtilogenyGUI VERSION 1.0.0 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_C_STANDARD 11) #C11
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#if (MSVC)
#    # Trying to force enable designated initializers
#    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
#    set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
#    #add_compiler_options(/std:c++latest) 
#endif()

option(ENABLE_SDL "Enable SDL for window creation or rendering." ON)
option(ENABLE_GLFW "Enable GFLW for window creation." ON)

option(ENABLE_OPENGL "Enable OpenGL rendering." ON)
option(ENABLE_DIRECTX "Enable DirectX rendering." ON)
option(ENABLE_VULKAN "Enable Vulkan rendering." OFF)
option(ENABLE_METAL "Enable Metal rendering." OFF)
option(ENABLE_BGFX "Enable BGFX rendering." ON)

if((NOT ENABLE_SDL) AND (NOT ENABLE_GLFW))
	message(FATAL_ERROR "Either SDL or GLFW must be enabled for window management.")
endif()

if(ENABLE_SDL)
	include(${UTILOGENY_DIR}/lib/sdl2/CMakeLists.txt)
	set(LINK_LIBS SDL2-static SDL2_ttf SDL2_image)
    set(SOURCES
        ${SOURCES}
        "window/SDLWindowImpl.h"
        "window/SDLWindowImpl.cpp"
        "renderer/sdl/SDLRenderer.h"
        "renderer/sdl/SDLRenderer.cpp"
    )
    add_compile_definitions(ENABLE_SDL)
endif()

if(ENABLE_GLFW)
	include(${UTILOGENY_DIR}/lib/glfw/CMakeLists.txt)
	set(LINK_LIBS ${LINK_LIBS} glfw)
	if (NOT (ENABLE_OPENGL OR ENABLE_DIRECTX OR ENABLE_VULKAN OR ENABLE_METAL OR ENABLE_BGFX))
		message(FATAL_ERROR "GLFW has no internal renderer and requires that at least one graphics API be enabled.")
	endif()
    set(SOURCES
        ${SOURCES}
        "window/GLFWWindowImpl.h"
        "window/GLFWWindowImpl.cpp"
    )
    add_compile_definitions(ENABLE_GLFW)
endif()

if(ENABLE_OPENGL)
	include(${UTILOGENY_DIR}/lib/glew/CMakeLists.txt)
	set(LINK_LIBS ${LINK_LIBS} libglew_static)
    set(SOURCES
        ${SOURCES}
        "renderer/opengl/OpenGLRenderer.h"
        "renderer/opengl/OpenGLRenderer.cpp"
    )
    add_compile_definitions(ENABLE_OPENGL)
endif()

if(ENABLE_VULKAN)
	include(${UTILOGENY_DIR}/lib/vulkan/CMakeLists.txt)
	set(LINK_LIBS ${LINK_LIBS} vulkan::vulkan)
    set(SOURCES
        ${SOURCES}
        "renderer/VulkanRenderer.h"
        "renderer/VulkanRenderer.cpp"
    )
    add_compile_definitions(ENABLE_VULKAN)
endif()

if(ENABLE_BGFX)
    include(${UTILOGENY_DIR}/lib/bgfx/CMakeLists.txt)
	set(LINK_LIBS ${LINK_LIBS} bgfx bx bimg) # example-framework meshoptimizer assimp
    set(SOURCES
        ${SOURCES}
        "renderer/bgfx/BgfxRenderer.h"
        "renderer/bgfx/BgfxRenderer.cpp"
        "renderer/bgfx/BgfxRendererImpl.h"
        "renderer/bgfx/BgfxRendererImpl.cpp"
        "renderer/bgfx/BgfxRendererContext.h"
        "renderer/bgfx/BgfxRendererContext.cpp"
        "renderer/bgfx/imgui/imgui_impl_bgfx.h"
        "renderer/bgfx/imgui/imgui_impl_bgfx.cpp"
    )
    add_compile_definitions(ENABLE_BGFX)
endif()

include(${UTILOGENY_DIR}/lib/imgui/CMakeLists.txt) # always enabled?
set(LINK_LIBS ${LINK_LIBS} ImGui)
set(SOURCES
    ${SOURCES}
    "renderer/ImGUIManager.h"
    "renderer/ImGUIManager.cpp"
)
add_compile_definitions(ENABLE_IMGUI)

include(${UTILOGENY_DIR}/lib/spdlog/CMakeLists.txt)
set(LINK_LIBS ${LINK_LIBS} spdlog)

set(SOURCES
    ${SOURCES}
    "Application.h"
    "Application.cpp"
    "ExecutableObject.h"
    "ExecutableObject.cpp"
    "ExecutableObjectImpl.h"
    "ExecutableObjectImpl.cpp"
    "ExtendedException.h"
    "ExtendedException.cpp"
    "main.cpp"
    "Object.h"
    "Object.cpp"
    "Settings.h"
    "Settings.cpp"
    "event/Event.h"
    "event/polym/Msg.h"
    "event/polym/Msg.cpp"
    "event/polym/Queue.h"
    "event/polym/Queue.cpp"
    "renderer/Color.h"
    "renderer/Color.cpp"
    "renderer/Renderer.h"
    "renderer/Renderer.cpp"
    "renderer/RendererTypes.h"
    "widget/widget.h"
    "widget/widget.cpp"
    "window/WindowingSystem.h"
    "window/Window.h"
    "window/Window.cpp"
    "window/WindowImpl.h"
    "window/WindowImpl.cpp"
    "window/WindowingSystem.cpp" 
    "renderer/RendererTypes.cpp")

#file(GLOB_RECURSE SOURCES "*.c" "*.cpp")
#file(GLOB_RECURSE HEADERS "*.h" "*.hpp")

# Create the executable
add_library(UtilogenyGUI ${SOURCES})

# Link the libraries
#add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(UtilogenyGUI PRIVATE ${LINK_LIBS})

function(print_target_properties target)

    # Get all propreties that cmake supports
    if(NOT CMAKE_PROPERTY_LIST)
        execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)
    
        # Convert command output into a CMake list
        string(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
        string(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
    endif()

    #message(STATUS "Original property list: ${CMAKE_PROPERTY_LIST}")
    set(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})

    #message(STATUS "CMake configuration types: ${CONFIG_TYPE}")

    # Fill in configuration entries and strip LOCATION entries
    set(CMAKE_PROPERTY_LIST_ALT "")
    foreach(property ${CMAKE_PROPERTY_LIST})
        string(FIND "${property}" "LOCATION" is_location)
        if(is_location EQUAL -1)
            string(FIND "${property}" "<CONFIG>" is_config)
            if(is_config GREATER -1)
                foreach(configtype ${CONFIG_TYPE})
                    string(REPLACE "<CONFIG>" "${configtype}" property_tmp ${property})
                    list(APPEND CMAKE_PROPERTY_LIST_ALT ${property_tmp})
                endforeach()
            else()
                list(APPEND CMAKE_PROPERTY_LIST_ALT ${property})
            endif()
        endif()
    endforeach()

    # Clear original property list
    set(CMAKE_PROPERTY_LIST "")

    #message(STATUS "Property list after filling in configurations: ${CMAKE_PROPERTY_LIST_ALT}")

    # Get list of languages supported by current project
    get_property(LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)

    # Fill in language entries
    foreach(property ${CMAKE_PROPERTY_LIST_ALT})
        string(FIND "${property}" "<LANG>" is_lang)
        if(is_lang GREATER -1)
            #message(STATUS "Had <LANG>: ${property}")
            foreach(lang ${LANGUAGES})
                string(REPLACE "<LANG>" "${lang}" property_tmp ${property})
                list(APPEND CMAKE_PROPERTY_LIST ${property_tmp})
            endforeach()
        else()
            list(APPEND CMAKE_PROPERTY_LIST ${property})
        endif()
    endforeach()

    # Clear alternative property list
    set(CMAKE_PROPERTY_LIST_ALT "")

    # Remove duplicates
    list(REMOVE_DUPLICATES CMAKE_PROPERTY_LIST)
    list(SORT CMAKE_PROPERTY_LIST)

    message(STATUS "Checking the following list of properties for target ${target}: ${CMAKE_PROPERTY_LIST}")

    if(NOT TARGET ${target})
      message(STATUS "There is no target named '${target}'")
      return()
    endif()

    foreach(property ${CMAKE_PROPERTY_LIST})
        get_property(was_set TARGET ${target} PROPERTY ${property} SET)
        if(was_set)
            get_target_property(value ${target} ${property})
            message(STATUS "${target} ${property} = ${value}")
        endif()
    endforeach()

    # Recursive for linked libraries
    get_target_property(linklibs ${target} LINK_LIBRARIES)
    list(REMOVE_DUPLICATES linklibs)

    if(linklibs)
        foreach(lib ${linklibs})
            print_target_properties(${lib})
        endforeach()
    endif()
endfunction()

# Push the current (NEW) CMake policy onto the stack, and apply the OLD policy.
print_target_properties(UtilogenyGUI)