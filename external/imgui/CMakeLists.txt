file(GLOB sources *.cpp)

set(ikura-ext-src
	${ikura-ext-src} ${sources}
	PARENT_SCOPE)

file(GLOB headers *.h)
install(FILES
	${headers}
	DESTINATION include/ikura/imgui)