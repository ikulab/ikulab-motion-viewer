file(GLOB NATIVE_WINDOW_SRC nativeWindow/*.cpp)
file(GLOB VIRTUAL_WINDOW_SRC virtualWindow/*.cpp)

add_library(ikura-window STATIC window.cpp ${NATIVE_WINDOW_SRC} ${VIRTUAL_WINDOW_SRC})

target_link_libraries(ikura-window PkgConfig::easyloggingpp)
target_link_libraries(ikura-window imgui)

file(GLOB headers ./*.hpp)
install(FILES ${headers}
	DESTINATION include/ikura/window)

file(GLOB native_window_headers nativeWindow/*.hpp)
install(FILES ${native_window_headers}
	DESTINATION include/ikura/window/nativeWindow)

file(GLOB virtual_window_headers virtualWindow/*.hpp)
install(FILES ${virtual_window_headers}
	DESTINATION include/ikura/window/virtualWindow)
