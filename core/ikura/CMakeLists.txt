if (APPLE)
    set(common_include_dir
            ${common_include_dir}
            /usr/local/include
    )
endif ()

# get all source files in ikura/
file(GLOB_RECURSE ikura_sources "*.cpp")

# build all files in ikura/
add_library(ikura STATIC ${ikura_sources})
# set include directory for ikura
target_include_directories(ikura PRIVATE
        ${PROJECT_SOURCE_DIR}/external
        ${PROJECT_SOURCE_DIR}/external/vk_mem_alloc
        ${PROJECT_SOURCE_DIR}/external/shaderc
        ${PROJECT_SOURCE_DIR}/external/libshaderc_util
)
# set compile options for ikura
target_compile_definitions(ikura PRIVATE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)

# set include directory for applications using ikura
target_include_directories(ikura INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/ikura>
        $<INSTALL_INTERFACE:${common_include_dir}>)
