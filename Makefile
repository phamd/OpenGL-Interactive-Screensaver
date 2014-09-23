CXX = gcc `pkg-config --cflags --libs gl` -I/usr/include/GL/ -lGLU -lglut -lm

3gc3_asg1: main.cpp containers.cpp
	$(CXX) $? -o $@

clean:
	rm 3gc3_asg1
