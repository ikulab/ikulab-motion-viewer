set(common_include_dir ${PROJECT_SOURCE_DIR}/core)

# get all source files in app/
file(GLOB_RECURSE imv_sources "*.cpp")

add_executable(ikulab-motion-viewer ${imv_sources})

target_include_directories(ikulab-motion-viewer PUBLIC "${common_include_dir}")
target_compile_features(ikulab-motion-viewer PRIVATE cxx_std_17)
