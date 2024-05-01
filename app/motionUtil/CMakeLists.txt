add_library(motionUtil STATIC ./bvhParser.cpp ./animator.cpp ./bvhExporter.cpp ./common.cpp)

target_link_libraries(motionUtil PRIVATE glm::glm)