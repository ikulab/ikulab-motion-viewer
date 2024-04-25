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
	add_compile_options(
		-std=c++17 -fdiagnostics-color=always -g
		-Wno-nullability-completeness
		-D_USE_MATH_DEFINES
		-DIS_WINDOWS
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
	add_compile_options(-DIMV_IS_DEBUG)
endif()
# }}}

# External Libraries Settings -------------------- {{{

find_package(PkgConfig REQUIRED)

find_package(glfw3 3.3 REQUIRED)
find_package(imgui REQUIRED)
find_package(tinyfiledialogs REQUIRED)
find_package(Vulkan REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(VulkanHeaders CONFIG REQUIRED)

pkg_check_modules(easyloggingpp easyloggingpp REQUIRED IMPORTED_TARGET)
# }}}

# build ikura
add_subdirectory(${CMAKE_SOURCE_DIR}/core/ikura)

# Link Libraries -------------------- {{{

# basic libraries
target_link_libraries(ikura PRIVATE ${CMAKE_DL_LIBS})

# external libraries
target_link_libraries(ikura PRIVATE glfw)
target_link_libraries(ikura PRIVATE Vulkan::Vulkan)
target_link_libraries(ikura PRIVATE PkgConfig::easyloggingpp)
target_link_libraries(ikura PRIVATE Vulkan::Headers)
target_link_libraries(ikura PRIVATE tinyfiledialogs)
target_link_libraries(ikura PRIVATE glm::glm)
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
