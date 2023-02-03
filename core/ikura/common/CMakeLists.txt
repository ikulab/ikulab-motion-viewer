add_library(ikura-common STATIC ./resourceDirectory.cpp)

install(FILES
	${PROJECT_SOURCE_DIR}/src/ikura/common/renderPrimitiveTypes.hpp
	${PROJECT_SOURCE_DIR}/src/ikura/common/uniformBufferInfo.hpp
	DESTINATION include/ikura/common)