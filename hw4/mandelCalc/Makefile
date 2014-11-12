#found at (http://stackoverflow.com/questions/7004702/how-can-i-create-a-makefile-for-c-projects-with-src-obj-and-bin-subdirectories)
#all credit listed below

# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#                thanks to http://stackoverflow.com/users/128940/beta
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = mandelCalc

#CC       = gcc
CC       = g++
# compiling flags here
#CFLAGS   = -std=c99 -Wall -I.
CFLAGS   = -Wall -I./include

#LINKER   = gcc -o
LINKER   = g++ -o
# linking flags here
#LFLAGS   = -Wall -I. -lm
LFLAGS   = -Wall -I./include

# change these to set the proper directories where each files shoould be
SRCDIR   = src
INCLUDEDIR = include
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCLUDEDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
OBJECTFILES := $(OBJDIR)/*
rm       = rm -f

$(BINDIR): $(OBJECTS)
	$(LINKER) $@/$(TARGET) $(LFLAGS) $(OBJECTFILES)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
