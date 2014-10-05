# Linux
SRC = main.cpp containers.cpp calculations.cpp 
EXE = 3gc3_asg1
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
LDLIBS = -lGL -lGLU -lglut -lm
zzzzzzz = gcc `pkg-config --cflags --libs gl` -I/usr/include/GL/ -lGL -lGLU -lglut -lm -std=c++11

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
    EXE = 3gc3_t1.exe
        LDFLAGS = -lfreeglut -lglu32 -lopengl32
endif

# OS X
OS := $(shell uname)
ifeq ($(OS), Darwin)
        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm $(EXE)
