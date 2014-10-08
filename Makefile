SRC = main.cpp containers.cpp calculations.cpp 
EXE = 3gc3_asg1

CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2 -g
LDLIBS = -lGL -lGLU -lglut -lm

OBJ = $(SRC:.cpp=.o)
EXT = 

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXT = .exe
	LDLIBS = -lfreeglut -lglu32 -lopengl32 -lm
endif

# OS X
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	LDLIBS = -framework Carbon -framework OpenGL -framework GLUT -lm
endif

$(EXE)$(EXT): $(OBJ)
	$(CXX) $^ -o $@ $(LDLIBS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

run:
	./$(EXE)$(EXT)

clean:
	rm ./$(OBJ) ./$(EXE)$(EXT)
