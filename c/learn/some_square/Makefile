# Makefile for 3D Spinning Cube with src/include structure

# Detect platform
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -I$(INCDIR)
LDFLAGS = 
TARGET = $(BINDIR)/spinning_cube

# Source files (without path)
SOURCES = main.c platform.c renderer.c math3d.c mesh.c shader.c input.c winsize_ctl.c

# Object files with obj directory
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

# Source files with src directory  
SRCFILES = $(SOURCES:%.c=$(SRCDIR)/%.c)

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    # Windows (MinGW)
    TARGET := $(TARGET).exe
    LDFLAGS += -lm
    RM = del /Q
    MKDIR = mkdir
    RMDIR = rmdir /S /Q
    PATHSEP = \\
else ifeq ($(UNAME_S),Linux)
    # Linux
    LDFLAGS += -lm
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
    PATHSEP = /
else ifeq ($(UNAME_S),Darwin)
    # macOS
    LDFLAGS += -lm
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
    PATHSEP = /
else
    # Default to Windows
    TARGET := $(TARGET).exe
    LDFLAGS += -lm
    RM = del /Q
    MKDIR = mkdir
    RMDIR = rmdir /S /Q
    PATHSEP = \\
endif

# Build rules
all: $(TARGET)

# Create directories if they don't exist
$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(BINDIR):
	$(MKDIR) $(BINDIR)

$(TARGET): $(OBJDIR) $(BINDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)

cleanall: clean
	$(RMDIR) $(OBJDIR) $(BINDIR)

install: $(TARGET)
	@echo "Program built successfully in $(BINDIR)/"

run: $(TARGET)
ifeq ($(OS),Windows_NT)
	$(TARGET)
else
	./$(TARGET)
endif

# Individual file dependencies
$(OBJDIR)/main.o: $(SRCDIR)/main.c $(INCDIR)/platform.h $(INCDIR)/renderer.h $(INCDIR)/winsize_ctl.h $(INCDIR)/math3d.h $(INCDIR)/mesh.h $(INCDIR)/shader.h $(INCDIR)/input.h
$(OBJDIR)/platform.o: $(SRCDIR)/platform.c $(INCDIR)/platform.h
$(OBJDIR)/renderer.o: $(SRCDIR)/renderer.c $(INCDIR)/renderer.h
$(OBJDIR)/math3d.o: $(SRCDIR)/math3d.c $(INCDIR)/math3d.h
$(OBJDIR)/mesh.o: $(SRCDIR)/mesh.c $(INCDIR)/mesh.h
$(OBJDIR)/shader.o: $(SRCDIR)/shader.c $(INCDIR)/shader.h $(INCDIR)/math3d.h
$(OBJDIR)/input.o: $(SRCDIR)/input.c $(INCDIR)/input.h
$(OBJDIR)/winsize_ctl.o: $(SRCDIR)/winsize_ctl.c $(INCDIR)/winsize_ctl.h

.PHONY: all clean cleanall install run help

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the spinning cube program"
	@echo "  clean    - Remove object files and executable"
	@echo "  cleanall - Remove object files, executable, and directories"
	@echo "  install  - Build the program"
	@echo "  run      - Build and run the program"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Project structure:"
	@echo "  $(SRCDIR)/     - Source files (.c)"
	@echo "  $(INCDIR)/     - Header files (.h)" 
	@echo "  $(OBJDIR)/     - Object files (created automatically)"
	@echo "  $(BINDIR)/     - Executable (created automatically)"