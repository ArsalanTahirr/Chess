# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -std=c++17 -Wall

# Target executable name
TARGET = chess.exe

# Source files
SOURCES = $(wildcard *.cpp)

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	del /Q $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)