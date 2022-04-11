CFLAGS = -std=c++17 -O2 -fdiagnostics-color=always -g
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi


app: main.cpp FirstTriangleApp.cpp FirstTriangleApp.hpp
	./compile.sh
	g++ $(CFLAGS) -o app main.cpp FirstTriangleApp.cpp $(LDFLAGS)

run: app
	./app

sample: sample.cpp
	./compile.sh
	g++ $(CFLAGS) -o sample sample.cpp $(LDFLAGS)

run_sample: sample
	./sample

.PHONY: clean

clean:
	rm -f app sample
	rm shaders/bin/*