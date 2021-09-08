GL_PKGS=glfw3 glew
FILES=main.c common/linear_algebra.c shaders/setup.c common/recursive_triangle.c common/camera.c
CFLAGS=-Wall -Wextra

run: cpp
	./main

c:
	gcc $(CGLAGS) `pkg-config --cflags $(GL_PKGS)` -lm -o main $(FILES) `pkg-config --libs $(GL_PKGS)`

cpp:
	g++ $(CGLAGS) `pkg-config --cflags $(GL_PKGS)` -lm -o main $(FILES) `pkg-config --libs $(GL_PKGS)`

shader: common/shader.cpp
	g++ -c `pkg-config --cflags $(GL_PKGS)` -lm -o shader.o common/shader.cpp `pkg-config --libs $(GL_PKGS)`
