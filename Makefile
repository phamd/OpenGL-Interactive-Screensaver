CXX = gcc `pkg-config --cflags --libs gl` -I/usr/include/GL/ -lGLU -lglut

3gc3_asg1: main.cpp containers.cpp containers.h
	$(CXX) $? -o $@

clean:
	rm 3gc3_asg1