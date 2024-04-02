include(FetchContent)

FetchContent_Declare(
  glew-fetch
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  # URL_HASH SHA256=D4FC82893CFB00109578E208A093F1F8A6EBE2996FFF3E6D8FAF22B0496E96D6
)

set( ONLY_LIBS ON CACHE BOOL "Do not build GLEW applications" )
set( glew-cmake_BUILD_SHARED OFF CACHE BOOL "Build the shared glew library" )

FetchContent_MakeAvailable(glew-fetch)