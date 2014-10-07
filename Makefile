# Linux
SRC = main.cpp containers.cpp calculations.cpp 
EXE = 3gc3_asg1
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
LDLIBS = -lGL -lGLU -lglut -lm
#old = gcc `pkg-config --cflags --libs gl` -I/usr/include/GL/ -lGL -lGLU -lglut -lm -std=c++11

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXE = $(EXE:=.exe)
	LDLIBS = -lfreeglut -lglu32 -lopengl32
endif

# OS X
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	LDLIBS = -framework Carbon -framework OpenGL -framework GLUT
endif

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm $(EXE)
