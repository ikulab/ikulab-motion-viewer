set(common_include_dir
	${PROJECT_SOURCE_DIR}/external
	${PROJECT_SOURCE_DIR}/external/vk_mem_alloc
	${PROJECT_SOURCE_DIR}/external/shaderc
	${PROJECT_SOURCE_DIR}/external/libshaderc_util
	${PROJECT_SOURCE_DIR}/external/tinyfiledialogs
	${PROJECT_SOURCE_DIR}/core
)

add_executable(ikulab-motion-viewer
	./main.cpp
	./app.cpp
	./callbacks.cpp
	./updateUi.cpp
	./resourceDirectory.cpp)
target_include_directories(ikulab-motion-viewer PUBLIC "${common_include_dir}")
target_compile_features(ikulab-motion-viewer PRIVATE cxx_std_17)

add_subdirectory(./context)
target_include_directories(context PUBLIC "${common_include_dir}")
target_link_libraries(ikulab-motion-viewer PRIVATE context)

add_subdirectory(./motionUtil)
target_include_directories(motionUtil PUBLIC "${common_include_dir}")
target_link_libraries(motionUtil PRIVATE glm::glm)
target_link_libraries(ikulab-motion-viewer PRIVATE motionUtil)

target_link_libraries(ikulab-motion-viewer PRIVATE imgui)