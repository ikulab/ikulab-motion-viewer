file(GLOB SOURCES *.cpp)
add_library(context STATIC ${SOURCES})

target_link_libraries(context PRIVATE glm::glm)

target_include_directories(context PUBLIC "${PROJECT_SOURCE_DIR}/includes")