main: main.cpp
	gcc `pkg-config --cflags --libs gl` -I/usr/include/GL/ -lGLU -lglut -o main main.cpp
