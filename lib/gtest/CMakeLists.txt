include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0  # Specify the commit, tag, or branch you want to use
)

# Hopefully this resolves the issue with inconsistent iterator debug levels between Google test and
# my projects, which is causing crashes when inserting items into unordered maps.
if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
endif()

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

# Make GoogleTest available for use
FetchContent_MakeAvailable(googletest)

#include_directories(${googletest_SOURCE_DIR}/googletest/include/gtest)

# Enable testing with CTest
enable_testing()

## Add your source files here
#add_executable(
#    your_test_target
#    your_test_file.cpp
#)

## Link GoogleTest to your test target
#target_link_libraries(
#    your_test_target
#    gtest_main
#)

## Add the test to be run by CTest
#add_test(
#    NAME your_test_name
#    COMMAND your_test_target
#)
