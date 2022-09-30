file(GLOB NATIVE_WINDOW_SRC nativeWindow/*.cpp)
file(GLOB VIRTUAL_WINDOW_SRC virtualWindow/*.cpp)

add_library(nativeWindow STATIC ${NATIVE_WINDOW_SRC})
add_library(virtualWindow STATIC ${VIRTUAL_WINDOW_SRC})
add_library(window STATIC window.cpp)

target_link_libraries(nativeWindow window)
target_link_libraries(virtualWindow window)