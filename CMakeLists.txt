cmake_minimum_required(VERSION 3.16)
project(
	ikura
	VERSION 0.1.0
	LANGUAGES CXX
)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Build Settings -------------------- {{{
if(WIN32)
	# set(CMAKE_CXX_COMPILER "clang++")
	add_compile_options(
		/std:c++17
		/D_USE_MATH_DEFINES
		/DIS_WINDOWS
		/D_ITERATOR_DEBUG_LEVEL=0
		/DIMGUI_DISABLE_WIN32_FUNCTIONS
		/MD
	)
elseif(LINUX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	add_compile_options(
		-std=c++17 -fdiagnostics-color=always -g
		-Wno-nullability-completeness
	)
elseif(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	add_compile_options(
		-std=c++17 -fdiagnostics-color=always -g
		-Wno-nullability-completeness
	)
endif()

if("${BUILD_TYPE}" STREQUAL "Debug")
	add_compile_options(-DDEBUG)
endif()
# }}}


# Common External Libraries Settings -------------------- {{{
if(LINUX)
	find_package(glfw3 3.3 REQUIRED)
elseif(WIN32)
	find_file(Glfw_LIB REQUIRED
		NAMES glfw3.lib
		PATHS ${CMAKE_SOURCE_DIR}/libs/glfw/win64-vc2022)
	find_library(Vulkan_LIB
		NAMES vulkan-1 vulkan
		PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan/win64)
elseif(APPLE)
	find_package(glfw3 3.3 REQUIRED)
	find_library(Vulkan_LIB NAMES libvulkan.dylib)
endif()
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
target_include_directories(ikura-ext-lib PUBLIC ${CMAKE_SOURCE_DIR}/external)
target_compile_definitions(ikura-ext-lib PUBLIC AUTO_INITIALIZE_EASYLOGGINGPP)
if(APPLE)
	target_include_directories(ikura-ext-lib PUBLIC "/usr/local/include")
endif()

# build ikura
add_subdirectory(${CMAKE_SOURCE_DIR}/core/ikura)

# link libshaderc static lib
if(LINUX OR APPLE)
	target_link_libraries(ikura
		PUBLIC ${PROJECT_SOURCE_DIR}/libs/shaderc/shaderc_combined.a)
elseif(WIN32)
	target_link_libraries(ikura
		PUBLIC ${PROJECT_SOURCE_DIR}/libs/shaderc/shaderc_combined.lib)
endif()

# link other dependencies
if(NOT "${Glfw_LIB}" STREQUAL "")
	target_link_libraries(ikura INTERFACE ${Glfw_LIB})
else()
	target_link_libraries(ikura INTERFACE glfw)
endif()

if(NOT "${Vulkan_LIB}" STREQUAL "")
	target_link_libraries(ikura INTERFACE ${Vulkan_LIB})
else()
	target_link_libraries(ikura INTERFACE vulkan)
endif()

target_link_libraries(ikura INTERFACE ${CMAKE_DL_LIBS})
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
