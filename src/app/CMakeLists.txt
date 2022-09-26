add_executable(app ./main.cpp ./app.cpp ./callbacks.cpp)
target_compile_features(app PRIVATE cxx_std_17)

add_subdirectory(./context)
target_link_libraries(app context)