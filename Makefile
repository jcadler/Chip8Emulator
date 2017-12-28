CPPFLAGS := -std=c++11
OBJECTS  := main.o
SRCFILES := main.cpp
DEPS = $(SRCFILES:.cpp=.d)

all : $(OBJECTS)
	$(CXX) $(CPPFLAGS) -o test $(OBJECTS) -lglut -lGLU -lGL -lsfml-audio

%.o : %.cpp
	@$(CXX) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)
