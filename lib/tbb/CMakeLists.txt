include(FetchContent)

FetchContent_Declare(
    tbb
    GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
    GIT_TAG        v2021.11.0 # Specify the version of TBB you want to use
)

set(TBB_TEST OFF)
set(TBB_EXAMPLES OFF)
set(TBBMALLOC_BUILD OFF)

FetchContent_MakeAvailable(tbb)