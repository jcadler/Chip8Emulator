BUILD_DIR := ./build
CPPFLAGS := -std=c++11
OBJECTS  := main.o core.o screen.o beep.o inst.o
OBJECT_LOCS = $(addprefix $(BUILD_DIR)/, $(OBJECTS))
SRCFILES := main.cpp core.cpp screen.cpp beep.cpp inst.cpp
DEPS = $(SRCFILES:.cpp=.d)

all : $(OBJECT_LOCS)
	$(CXX) $(CPPFLAGS) -o test $(OBJECT_LOCS) -lglut -lGLU -lGL -lsfml-audio

$(BUILD_DIR)/%.o : %.cpp
	@$(CXX) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)
