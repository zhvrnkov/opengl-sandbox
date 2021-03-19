GL_PKGS=glfw3 glew

c:
	gcc `pkg-config --cflags $(GL_PKGS)` -lm -o main main.c `pkg-config --libs $(GL_PKGS)`

shader: common/shader.cpp
	g++ -c `pkg-config --cflags $(GL_PKGS)` -lm -o shader.o common/shader.cpp `pkg-config --libs $(GL_PKGS)`
