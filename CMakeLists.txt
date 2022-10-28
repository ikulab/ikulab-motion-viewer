cmake_minimum_required(VERSION 3.16)
project(
	ikulab-motion-viewer
	VERSION 0.1.0
	LANGUAGES CXX
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(
	-std=c++17 -fdiagnostics-color=always -g
	-Wno-nullability-completeness
)

if("${BUILD_TYPE}" STREQUAL "Release")
	add_compile_options(-DNODEBUG)
endif()

# external libraries ----------
find_package(glfw3 3.3 REQUIRED)
add_subdirectory(${PROJECT_SOURCE_DIR}/ikura)
add_subdirectory(${PROJECT_SOURCE_DIR}/includes/tinyfiledialogs)

# build and link ----------
add_subdirectory(./src)
target_link_libraries(ikulab-motion-viewer tinyfiledialogs)
target_link_libraries(ikulab-motion-viewer ikura glfw)

# TODO: delete them
target_link_libraries(motionUtil ikura)
target_link_libraries(context ikura)

# install ----------
install(TARGETS ikulab-motion-viewer)

# font
install(DIRECTORY ${PROJECT_SOURCE_DIR}/fonts
	DESTINATION ${CMAKE_INSTALL_DATADIR}local/share/ikulab-motion-viewer)

# CPack config
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VENDOR "ikulab / CaffeineFree")
set(CPACK_PACKAGE_NAME "ikulab-motion-viewer")
set(CPACK_PACKAGE_CONTACT "caffeine0coffee@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "The BVH Motion Viewer powerd by ikura Graphics Engine.")

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libvulkan1, libglfw3, vulkan-validationlayers")

include(CPack)