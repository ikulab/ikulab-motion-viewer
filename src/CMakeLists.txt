add_executable(ikulab-motion-viewer ./main.cpp ./app.cpp ./callbacks.cpp ./updateUi.cpp)
target_compile_features(ikulab-motion-viewer PRIVATE cxx_std_17)

add_subdirectory(./context)
target_link_libraries(ikulab-motion-viewer context)

add_subdirectory(./motionUtil)
target_link_libraries(ikulab-motion-viewer motionUtil)

target_include_directories(ikulab-motion-viewer PUBLIC ${PROJECT_SOURCE_DIR}/includes/)
target_include_directories(ikulab-motion-viewer PUBLIC ${PROJECT_SOURCE_DIR}/includes/easylogging++)
target_include_directories(ikulab-motion-viewer PUBLIC ${PROJECT_SOURCE_DIR}/includes/tinyfiledialogs)