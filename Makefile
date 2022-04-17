CFLAGS = -std=c++17 -fdiagnostics-color=always -g
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
INCLUDE_PATH = ./includes

release: main.cpp app.cpp app.hpp shaders/shader.*
	./compile.sh
	g++ $(CFLAGS) -O3 -o app main.cpp app.cpp $(LDFLAGS) -I$(INCLUDE_PATH)

app: main.cpp app.cpp app.hpp shaders/shader.*
	./compile.sh
	g++ $(CFLAGS) -O2 -o app main.cpp app.cpp $(LDFLAGS) -I$(INCLUDE_PATH)

run: app
	./app

sample: sample.cpp
	./compile.sh
	g++ $(CFLAGS) -o sample sample.cpp $(LDFLAGS) -I$(INCLUDE_PATH)

run_sample: sample
	./sample

.PHONY: clean

clean:
	rm -f app sample
	rm shaders/bin/*