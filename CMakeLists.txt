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
# link ikura to ikulab-motion-viewer
target_link_libraries(ikulab-motion-viewer PRIVATE ikura)

# find / link external libraries
# libraries are installed via vcpkg
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(ikulab-motion-viewer PRIVATE tinyfiledialogs::tinyfiledialogs)
target_link_libraries(ikulab-motion-viewer PRIVATE glm::glm)

# ------------------------------------------------------------
# Config installer
# ------------------------------------------------------------

include(CPack)

# prepare MacOS bundle
if (APPLE)
    # create bundle (and install binary and Info.plist)
    set_target_properties(ikulab-motion-viewer PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/assets/macos/Info.plist
    )
    # set rpath
    set_target_properties(ikulab-motion-viewer PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
    )

    install(TARGETS ikulab-motion-viewer BUNDLE DESTINATION .)

    # install files

    # startup script
    install(FILES ${PROJECT_SOURCE_DIR}/assets/macos/startup.sh
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/app/ikulab-motion-viewer.app/Contents/MacOS
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)

    # vulkan dynamic libraries
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/assets/macos/Frameworks
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/app/ikulab-motion-viewer.app/Contents
        FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
    # vulkan library settings
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/assets/macos/vulkan
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/app/ikulab-motion-viewer.app/Contents/Resources)

    # icon
    install(FILES ${PROJECT_SOURCE_DIR}/assets/macos/icon.icns
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/app/ikulab-motion-viewer.app/Contents/Resources)
    # fonts
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/assets/fonts
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/app/ikulab-motion-viewer.app/Contents/Resources)
endif ()


# CPack config
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VENDOR "ikulab / CaffeineFree")
set(CPACK_PACKAGE_NAME "ikulab-motion-viewer")
set(CPACK_PACKAGE_CONTACT "dev@caffeinezero.info")
set(CPACK_PACKAGE_DESCRIPTION "The BVH Motion Viewer powered by ikura Graphics Engine.")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")

# CPack config for MacOS
set(CPACK_BUNDLE_NAME "ikulab-motion-viewer")
set(CPACK_BUNDLE_PLIST "${PROJECT_SOURCE_DIR}/Info.plist")
set(CPACK_BUNDLE_ICON "${PROJECT_SOURCE_DIR}/icon.icns")
set(CPACK_BUNDLE_STARTUP_COMMAND "${PROJECT_SOURCE_DIR}/build/app/startup.sh")

