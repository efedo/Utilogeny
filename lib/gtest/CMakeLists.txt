include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.17.0  # Specify the commit, tag, or branch you want to use
)

#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
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
