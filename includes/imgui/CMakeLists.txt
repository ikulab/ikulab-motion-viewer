file(GLOB SOURCES *.cpp)
add_library(imgui STATIC ${SOURCES})

target_include_directories(imgui INTERFACE ../)