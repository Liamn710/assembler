# Compiler settings
CC = gcc
CFLAGS = -ansi -pedantic -Wall

# Source files
SRCS = assembler.c front.c macro_handle.c
HEADERS = front.h macro_handle.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = assembler

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Run the assembler
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run