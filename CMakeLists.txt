cmake_minimum_required(VERSION 3.16)
project(
	ikura
	VERSION 0.1.0
	LANGUAGES CXX
)

# Build Settings -------------------- {{{
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_compile_options(
	-std=c++17 -fdiagnostics-color=always -g
	-Wno-nullability-completeness
)

if("${BUILD_TYPE}" STREQUAL "Release")
	add_compile_options(-DNODEBUG)
endif()
# }}}


# Common External Libraries Settings -------------------- {{{
find_package(Vulkan)
find_package(glfw3 3.3 REQUIRED)
# }}}


# Build ikura Core -------------------- {{{
# put the list of external lib's c/cpp files into ikura-ext-src
set(ikura-ext-src "")
add_subdirectory(${CMAKE_SOURCE_DIR}/external/imgui)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/easylogging++)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/vk_mem_alloc)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/shaderc)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/libshaderc_util)

# combine external libs into a static lib
add_library(ikura-ext-lib STATIC ${ikura-ext-src})
target_compile_definitions(ikura-ext-lib PUBLIC AUTO_INITIALIZE_EASYLOGGINGPP)
if(APPLE)
	target_include_directories(ikura-ext-lib PUBLIC "/usr/local/include")
endif()

# build ikura
add_subdirectory(${CMAKE_SOURCE_DIR}/core/ikura)

# link libshaderc static lib
target_link_libraries(ikura
	PUBLIC ${PROJECT_SOURCE_DIR}/libs/shaderc/libshaderc_combined.a)
# link other dependencies
target_link_libraries(ikura INTERFACE vulkan glfw ${CMAKE_DL_LIBS})
target_link_libraries(ikura INTERFACE ikura-ext-lib)
# }}}


# Build App -------------------- {{{
# external libs depends on app
add_subdirectory(${CMAKE_SOURCE_DIR}/external/tinyfiledialogs)

# build app
add_subdirectory(${CMAKE_SOURCE_DIR}/app)

# link libs
target_link_libraries(ikulab-motion-viewer ikura)
target_link_libraries(ikulab-motion-viewer tinyfiledialogs)
# }}}
