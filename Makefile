CXX = gcc `pkg-config --cflags --libs gl` -I/usr/include/GL/ -lGL -lGLU -lglut -lm -std=c++11

3gc3_asg1: main.cpp containers.cpp calculations.cpp
	$(CXX) $? -o $@

clean:
	rm 3gc3_asg1
