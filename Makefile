INCLUDES:=puzzle_piece

CXX := g++

CXXFLAGS := -std=c++17 -Wall -O2 

PROJECTNAME := puzzle_solver

OBJ := $(subst '.cpp','.out',$(shell find . -name '*.cpp'))

all: $(PROJECTNAME)
	./$(PROJECTNAME)

$(PROJECTNAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.out: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
