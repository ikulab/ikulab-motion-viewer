cmake_minimum_required(VERSION 3.16)
project(ikura VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    set(common_include_dir
            ${common_include_dir}
            /usr/local/include
    )
endif ()

# ------------------------------------------------------------
# prepare dependencies
# ------------------------------------------------------------

find_package(PkgConfig REQUIRED)

# packages installed via vcpkg
find_package(VulkanHeaders CONFIG REQUIRED)
find_package(VulkanLoader CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
pkg_check_modules(easyloggingpp easyloggingpp REQUIRED IMPORTED_TARGET)
find_package(glslang CONFIG REQUIRED)

# packages under external/ directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/ikura_ext_imgui)

# ------------------------------------------------------------
# build ikura
# ------------------------------------------------------------

# get all source files in ikura/
file(GLOB_RECURSE ikura_sources "*.cpp")

# build all files in ikura/
add_library(ikura STATIC ${ikura_sources})
# set compile options for ikura
target_compile_definitions(ikura PRIVATE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)
target_compile_definitions(ikura PRIVATE VMA_STATIC_VULKAN_FUNCTIONS=0 VMA_DYNAMIC_VULKAN_FUNCTIONS=1)

# set include directory for applications using ikura
target_include_directories(ikura INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/ikura>
        $<INSTALL_INTERFACE:${common_include_dir}>)

# ------------------------------------------------------------
# link external libraries
# ------------------------------------------------------------

target_link_libraries(ikura PUBLIC ${CMAKE_DL_LIBS})

# packages installed via vcpkg
target_link_libraries(ikura PUBLIC Vulkan::Headers)
target_link_libraries(ikura PUBLIC Vulkan::Loader)
target_link_libraries(ikura PUBLIC GPUOpen::VulkanMemoryAllocator)
target_link_libraries(ikura PUBLIC glfw)
target_link_libraries(ikura PUBLIC PkgConfig::easyloggingpp)
target_link_libraries(ikura PUBLIC glslang::glslang glslang::glslang-default-resource-limits glslang::SPIRV glslang::SPVRemapper)

# packages under external/ directory
target_include_directories(ikura PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_link_libraries(ikura PUBLIC ikura_ext_imgui)
