# External package
find_package(glfw3 3.3 REQUIRED)

# add libraries
add_library(appEngine STATIC appEngine.cpp)

file(GLOB RENDER_ENGINE_SOURCE renderEngine/*.cpp)
add_library(renderEngine STATIC ${RENDER_ENGINE_SOURCE})

target_compile_definitions(renderEngine PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)