# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = 

# Target executable
TARGET = listy

# Directories
SRCDIR = src
INCLUDEDIR = include

# Source and object files
SRCS = $(SRCDIR)/listy.c $(SRCDIR)/main.c $(SRCDIR)/todo_io.c
OBJS = $(SRCS:.c=.o)

# Installation directory
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c $(INCLUDEDIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@

# Install the program
install: $(TARGET)
	mkdir -p $(BINDIR)
	cp $(TARGET) $(BINDIR)/

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean install
