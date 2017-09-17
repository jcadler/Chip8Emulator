CPPFLAGS = -std=c++11

all : main.o
	g++ $(CPPFLAGS) -o test main.o -lglut -lGLU -lGL

main.o : main.cpp
	g++ $(CPPFLAGS) -c main.cpp 
