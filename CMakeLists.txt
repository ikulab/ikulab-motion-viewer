cmake_minimum_required(VERSION 3.16)
project(ikulab-motion-viewer VERSION 0.1.0 LANGUAGES CXX)

# ------------------------------------------------------------
# build settings for each platform
# ------------------------------------------------------------

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()

if (WIN32)
    add_compile_options(
            -std=c++17 -fdiagnostics-color=always -g
            -Wno-nullability-completeness
            -D_USE_MATH_DEFINES
            -DIS_WINDOWS
    )
elseif (LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    add_compile_options(
            -std=c++17 -fdiagnostics-color=always -g
            -Wno-nullability-completeness
    )
elseif (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    add_compile_options(
            -std=c++17 -fdiagnostics-color=always -g
            -Wno-nullability-completeness
    )
endif ()

# ------------------------------------------------------------
# build ikulab-motion-viewer
# ------------------------------------------------------------

# set IMV_IS_DEBUG flag
if ("${BUILD_TYPE}" STREQUAL "Debug")
    add_compile_options(-DIMV_IS_DEBUG)
endif ()

# build ikura
add_subdirectory(${CMAKE_SOURCE_DIR}/core/ikura)
# build ikulab-motion-viewer
add_subdirectory(${CMAKE_SOURCE_DIR}/app)
# link
target_link_libraries(ikulab-motion-viewer PRIVATE ikura)

# find / link external libraries
# libraries are installed via vcpkg
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(ikulab-motion-viewer PRIVATE tinyfiledialogs::tinyfiledialogs)
target_link_libraries(ikulab-motion-viewer PRIVATE glm::glm)
