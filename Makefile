# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++20

# Output executable
TARGET = prog

# Source files
SRCS = src/main.cpp src/EnvException.cpp src/InvalidEnvException.cpp src/MissingEnvException.cpp

# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Default rule (build the executable)
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule (removes object files and the executable)
clean:
	rm -f $(OBJS) $(TARGET)
