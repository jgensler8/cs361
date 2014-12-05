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
TARGET   = raceTest

#CC       = gcc
CC       = gcc
# compiling flags here
#CFLAGS   = -std=c99 -Wall -I.
CFLAGS   = -Wall -I./include

#LINKER   = gcc -o
LINKER   = gcc -o
# linking flags here
#LFLAGS   = -Wall -I. -lm
LFLAGS   = -Wall -I./include -pthread

# change these to set the proper directories where each files shoould be
SRCDIR   = src
INCLUDEDIR = include
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCLUDEDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJECTFILES := $(OBJDIR)/*
rm       = rm -f


$(BINDIR): $(OBJECTS)
	$(LINKER) $@/$(TARGET) $(LFLAGS) $(OBJECTFILES)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
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
