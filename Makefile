CPPFLAGS := -std=c++11
OBJECTS  := main.o core.o screen.o beep.o
SRCFILES := main.cpp core.cpp screen.cpp beep.cpp
DEPS = $(SRCFILES:.cpp=.d)

all : $(OBJECTS)
	$(CXX) $(CPPFLAGS) -o test $(OBJECTS) -lglut -lGLU -lGL -lsfml-audio

%.o : %.cpp
	@$(CXX) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)
