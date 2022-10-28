set(ikura-ext-src
	${ikura-ext-src} ${PROJECT_SOURCE_DIR}/includes/easylogging++/easylogging++.cc
	PARENT_SCOPE)

install(FILES
	${PROJECT_SOURCE_DIR}/includes/easylogging++/easylogging++.h
	${PROJECT_SOURCE_DIR}/includes/easylogging++/logLevels.hpp
	DESTINATION include/ikura/easylogging++)