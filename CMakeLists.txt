cmake_minimum_required(VERSION 3.16)
project(
	ikulab-motion-viewer
	VERSION 0.1.0
	LANGUAGES CXX
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(
	-std=c++17 -fdiagnostics-color=always -g
)

option(USE_CLANG "set CXX compiler to Clang" TRUE)

if(USE_CLANG)
	set(CMAKE_CXX_COMPILER clang++)
	set(CMAKE_C_COMPILER clang)
	add_compile_options(
		-Wno-nullability-completeness
	)
else()
	set(CMAKE_CXX_COMPILER gcc)
	set(CMAKE_C_COMPILER gcc)
endif()

if("${BUILD_TYPE}" STREQUAL "Release")
	add_compile_options(-DNODEBUG)
endif()

# external libraries ----------
find_package(glfw3 3.3 REQUIRED)
find_package(ikura)

add_subdirectory(${CMAKE_SOURCE_DIR}/includes/tinyfiledialogs)

# build and link ----------
add_subdirectory(./src)

# app dependencies
target_link_libraries(ikulab-motion-viewer tinyfiledialogs)
target_link_libraries(ikulab-motion-viewer ikura glfw)

# TODO: delete them
target_link_libraries(motionUtil ikura)
target_link_libraries(context ikura)

# install ----------
install(TARGETS ikulab-motion-viewer)
# shared libs
install(CODE [[
	file(GET_RUNTIME_DEPENDENCIES
		RESOLVED_DEPENDENCIES_VAR resolve_files
		UNRESOLVED_DEPENDENCIES_VAR unresolve_files
		EXECUTABLES $<TARGET_FILE:ikulab-motion-viewer>
	)

	message(VERBOSE "resolved: ${resolve_files}")
	message(VERBOSE "unresolved: ${unresolve_files}")

	list(LENGTH unresolved unresolved_len)
	if("${unresolved_len}" GREATER 0)
		message(WARNING "${unresolved_len} external libraries are not resolved.")
	endif()

	foreach(res ${resolve_files})
		message(VERBOSE "installing: ${res}")
		file(INSTALL
			DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ikulab-motion-viewer"
			TYPE SHARED_LIBRARY
			FOLLOW_SYMLINK_CHAIN
			FILES "${res}"
		)
	endforeach()
]])

include(CPack)