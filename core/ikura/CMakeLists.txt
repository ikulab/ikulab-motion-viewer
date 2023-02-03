set(common_include_dir
	${PROJECT_SOURCE_DIR}/external
	${PROJECT_SOURCE_DIR}/external/easylogging++
	${PROJECT_SOURCE_DIR}/external/vk_mem_alloc
	${PROJECT_SOURCE_DIR}/external/shaderc
	${PROJECT_SOURCE_DIR}/external/libshaderc_util
)

if(APPLE)
	set(common_include_dir
		${common_include_dir}
		/usr/local/include
	)
endif()

add_library(ikura STATIC init.cpp)
target_link_directories(ikura PUBLIC "${common_include_dir}")

add_subdirectory(./common)
target_include_directories(ikura-common PUBLIC "${common_include_dir}")
target_link_libraries(ikura PUBLIC ikura-common)

add_subdirectory(./window)
target_include_directories(ikura-nativeWindow PUBLIC "${common_include_dir}")
target_include_directories(ikura-virtualWindow PUBLIC "${common_include_dir}")
target_include_directories(ikura-window PUBLIC "${common_include_dir}")
target_link_libraries(ikura PUBLIC ikura-nativeWindow ikura-virtualWindow)

add_subdirectory(./engine)
target_include_directories(ikura-appEngine PUBLIC "${common_include_dir}")
target_include_directories(ikura-renderEngine PUBLIC "${common_include_dir}")
target_link_libraries(ikura PUBLIC ikura-appEngine ikura-renderEngine)

add_subdirectory(./renderComponent)
target_include_directories(ikura-renderComponent PUBLIC "${common_include_dir}")
target_link_libraries(ikura PUBLIC ikura-renderComponent)

add_subdirectory(./misc)
target_include_directories(ikura-misc PUBLIC "${common_include_dir}")
target_link_libraries(ikura PUBLIC ikura-misc)

add_subdirectory(./shape)
target_include_directories(ikura-shape PUBLIC "${common_include_dir}")
target_link_libraries(ikura PUBLIC ikura-shape)

target_include_directories(ikura INTERFACE
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include/ikura>
	$<INSTALL_INTERFACE:${common_include_dir}>)

install(FILES
	${PROJECT_SOURCE_DIR}/src/ikura/ikuraConfig.h
	${PROJECT_SOURCE_DIR}/src/ikura/init.hpp
	DESTINATION include/ikura)
