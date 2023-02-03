add_library(ikura-misc STATIC initVulkanHppDispatchLoader.cpp)
target_compile_definitions(ikura-misc PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)

install(FILES
	${PROJECT_SOURCE_DIR}/src/ikura/misc/initVulkanHppDispatchLoader.hpp
	DESTINATION include/ikura/misc)