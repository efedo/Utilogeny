include(FetchContent)

# Fetch BGFX (CMake repo)
FetchContent_Declare(
    bgfx
    GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake
    GIT_TAG        master # or specific tag/commit
)

# Configure fetch settings for BGFX (if needed)
# This is where you can add additional commands specific to BGFX
# ...

#set(BGFX_LIBRARY_TYPE "static")
set(BGFX_BUILD_EXAMPLES OFF)
set(BGFX_BUILD_TESTS OFF)
set(BGFX_CONFIG_MULTITHREADED ON)

message(STATUS "Beginning FetchContent_MakeAvailable for bgfx.")

FetchContent_MakeAvailable(bgfx)

message(STATUS "Completed FetchContent_MakeAvailable for bgfx.")