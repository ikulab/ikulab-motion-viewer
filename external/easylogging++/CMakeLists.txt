set(ikura-ext-src
	${ikura-ext-src} ${PROJECT_SOURCE_DIR}/external/easylogging++/easylogging++.cc
	PARENT_SCOPE)

install(FILES
	${PROJECT_SOURCE_DIR}/external/easylogging++/easylogging++.h
	${PROJECT_SOURCE_DIR}/external/easylogging++/logLevels.hpp
	DESTINATION include/ikura/easylogging++)
