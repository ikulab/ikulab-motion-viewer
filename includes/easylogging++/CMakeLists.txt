set(ikura-ext-src
	${ikura-ext-src} ${CMAKE_SOURCE_DIR}/includes/easylogging++/easylogging++.cc
	PARENT_SCOPE)

install(FILES
	${CMAKE_SOURCE_DIR}/includes/easylogging++/easylogging++.h
	${CMAKE_SOURCE_DIR}/includes/easylogging++/logLevels.hpp
	DESTINATION include/ikura/easylogging++)