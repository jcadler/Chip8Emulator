CPPFLAGS = -std=c++11
OBJECTS = main.o

all : $(OBJECTS)
	$(CXX) $(CPPFLAGS) -o test $(OBJECTS) -lglut -lGLU -lGL

%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

main.o: main.cpp core.h
	$(CXX) $(CPPFLAGS) -c main.cpp -o main.o
