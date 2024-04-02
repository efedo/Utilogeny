# FindSDL2.cmake

# GLFW
include(FetchContent)

FetchContent_Declare(
  glfw-fetch
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 2c1d31009fdc527d5a44ff887659a25ecbf1f7f0
  #GIT_TAG 3.4.0
  #Require at least glfw 3.4, but not officially released yet
)

set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_TESTS    OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_DOCS     OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_INSTALL  OFF CACHE BOOL  "GLFW lib only" )

FetchContent_MakeAvailable(glfw-fetch)