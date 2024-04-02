# FindSDL2.cmake

# Migrate to SDL3 compatibility layer eventually?
# https://github.com/libsdl-org/sdl2-compat

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
#if(PIE)
#	set(SDL_STATIC_PIC ON)
#endif()
set(SDL_TEST_ENABLED_BY_DEFAULT OFF)

FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.30.1
)

FetchContent_Declare(SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        release-2.22.0
)

FetchContent_Declare(SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG        release-2.8.2
)


message(STATUS "Beginning FetchContent_MakeAvailable for SDL2.")

FetchContent_MakeAvailable(SDL2)
FetchContent_MakeAvailable(SDL_ttf)
FetchContent_MakeAvailable(SDL_image)

message(STATUS "Completed FetchContent_MakeAvailable for SDL2.")