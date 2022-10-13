file(GLOB SOURCES *.cpp)
add_library(context STATIC ${SOURCES})

target_include_directories(context PUBLIC "${PROJECT_SOURCE_DIR}/includes")