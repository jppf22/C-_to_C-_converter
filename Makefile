# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Target executable
TARGET = main

# For deleting the target
TARGET_DEL = main

# Source files
SRCS = source/main.cpp source/file_handler.cpp source/tokenizer.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -rf $(TARGET_DEL) $(OBJS)