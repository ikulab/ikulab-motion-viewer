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

# installed via vcpkg
find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
pkg_check_modules(easyloggingpp easyloggingpp REQUIRED IMPORTED_TARGET)

# }}}

# Build Ikura -------------------- {{{

add_subdirectory(${CMAKE_SOURCE_DIR}/core/ikura)

# link external libraries
target_link_libraries(ikura PRIVATE Vulkan::Vulkan)
target_link_libraries(ikura PRIVATE GPUOpen::VulkanMemoryAllocator)
target_link_libraries(ikura PRIVATE PkgConfig::easyloggingpp)
target_link_libraries(ikura PRIVATE glm::glm)
target_link_libraries(ikura PRIVATE imgui::imgui)

target_link_libraries(ikura PRIVATE ${CMAKE_DL_LIBS})

# }}}

# Build ikulab-motion-viewer -------------------- {{{

add_subdirectory(${CMAKE_SOURCE_DIR}/app)

target_link_libraries(ikulab-motion-viewer PRIVATE ikura)

target_link_libraries(ikulab-motion-viewer PRIVATE tinyfiledialogs::tinyfiledialogs)
target_link_libraries(ikulab-motion-viewer PRIVATE glfw)

# }}}
