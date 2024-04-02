# FindVulkanHeaders.cmake

include(FetchContent)

set(VULKAN_HEADERS_VERSION "1.2.196") # Specify the desired version of Vulkan headers here

FetchContent_Declare(
  VulkanHeaders
  GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
  GIT_TAG "v${VULKAN_HEADERS_VERSION}"
)

FetchContent_GetProperties(VulkanHeaders)
if(NOT vulkanheaders_POPULATED)
  FetchContent_Populate(VulkanHeaders)
  set(VULKAN_HEADERS_INSTALL_DIR OFF CACHE BOOL "" FORCE)
  add_subdirectory(${vulkanheaders_SOURCE_DIR} ${vulkanheaders_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

find_path(VULKAN_INCLUDE_DIR vulkan/vulkan.h
  HINTS
    ENV VULKAN_HEADERS_DIR
    ${vulkanheaders_SOURCE_DIR}/include
  PATH_SUFFIXES vulkan
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VulkanHeaders
  FOUND_VAR VULKANHEADERS_FOUND
  REQUIRED_VARS VULKAN_INCLUDE_DIR
)

if(VULKAN_HEADERS_FOUND)
  set(VULKAN_INCLUDE_DIRS ${VULKAN_INCLUDE_DIR})
endif()