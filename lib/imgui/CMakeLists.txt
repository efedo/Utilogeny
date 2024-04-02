# Fetch ImGui
include(FetchContent)

FetchContent_Declare(
    imgui-fetch
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.90.4
)

FetchContent_MakeAvailable(imgui-fetch)

# ImGui target
add_library(ImGui
    ${imgui-fetch_SOURCE_DIR}/imgui.cpp
    ${imgui-fetch_SOURCE_DIR}/imgui_demo.cpp
    ${imgui-fetch_SOURCE_DIR}/imgui_draw.cpp
    ${imgui-fetch_SOURCE_DIR}/imgui_tables.cpp
    ${imgui-fetch_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui-fetch_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui-fetch_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
    ${imgui-fetch_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui-fetch_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(ImGui PUBLIC ${imgui-fetch_SOURCE_DIR})

set(imgui_link_libs)
if(ENABLE_SDL)
	set(imgui_link_libs ${imgui_link_libs} SDL2-static)
endif()

if(ENABLE_GLFW)
	set(imgui_link_libs ${imgui_link_libs} glfw)
endif()

target_link_libraries(ImGui ${imgui_link_libs})