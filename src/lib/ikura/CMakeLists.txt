add_library(ikura INTERFACE)

set(common_include_dir
	"${PROJECT_SOURCE_DIR}/includes"
	"${PROJECT_SOURCE_DIR}/includes/tinyfiledialogs"
	"${PROJECT_SOURCE_DIR}/includes/easylogging++"
	"${PROJECT_SOURCE_DIR}/includes/vk_mem_alloc"
)

add_subdirectory(./window)
target_include_directories(nativeWindow PUBLIC "${common_include_dir}")
target_include_directories(virtualWindow PUBLIC "${common_include_dir}")
target_include_directories(window PUBLIC "${common_include_dir}")
target_link_libraries(ikura INTERFACE nativeWindow virtualWindow)

add_subdirectory(./engine)
target_include_directories(appEngine PUBLIC "${common_include_dir}")
target_include_directories(renderEngine PUBLIC "${common_include_dir}")
target_link_libraries(ikura INTERFACE appEngine renderEngine)

add_subdirectory(./renderComponent)
target_include_directories(renderComponent PUBLIC "${common_include_dir}")
target_link_libraries(ikura INTERFACE renderComponent)

add_subdirectory(./misc)
target_include_directories(initVulkanHppDispatchLoader PUBLIC "${common_include_dir}")
target_link_libraries(ikura INTERFACE initVulkanHppDispatchLoader)

add_subdirectory(./shape)
target_include_directories(shape PUBLIC  "${common_include_dir}")
target_link_libraries(ikura INTERFACE shape)

target_include_directories(ikura INTERFACE ./)