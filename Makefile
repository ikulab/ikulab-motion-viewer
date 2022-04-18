Cflags = -std=c++17 -fdiagnostics-color=always -g
LdFlags = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SrcPaths = src/main.cpp src/app.cpp
Ingredients = $(SrcPaths) src/app.hpp shaders/shader.*
IncludePath = ./includes

release: $(Ingredients)
	./compile.sh
	g++ $(Cflags) -O3 -o bin/app $(SrcPaths) $(LdFlags) -I$(IncludePath)

app: $(Ingredients)
	./compile.sh
	g++ $(Cflags) -O2 -o bin/app $(SrcPaths) $(LdFlags) -I$(IncludePath)

run: app
	bin/app

.PHONY: clean

clean:
	rm bin/app
	rm shaders/bin/*