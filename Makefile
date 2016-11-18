################
# DEFINITIONS
################

# This will be the name of the executable
TARGET:=interleave_test

# Directories
OBJDIR:=build
SRCDIR:=src
BINDIR:=bin

# ANSI color codes
# Only used for fancy output
NO_COLOR=\x1b[0m
RED:=\x1b[31m
GREEN:=\x1b[32m
YELLOW:=\x1b[33m

################
# TOOLCHAIN
################

CC:=gcc
LD:=gcc

################
# FLAGS
################

INCLUDES:= -Iinc/
LIBS:=

CFLAGS:=-std=c99 -Wall $(INCLUDES)
LDFLAGS:=$(LIBS)

################
# FILES
################

CSRC:= $(addprefix $(SRCDIR)/, \
interleave.c \
interleave_test.c \
)

C_FILES := $(notdir $(CSRC))
C_PATHS := $(dir $(CSRC))
C_OBJS := $(addprefix $(OBJDIR)/, $(C_FILES:.c=.o))
OBJS = $(C_OBJS)

vpath %.c $(C_PATHS)

################
# BUILD RULES
################

.PHONY : all run clean purge

all : $(BINDIR)/$(TARGET)

TOTAL_IMAGES := 400
INTERLEAVED := 8
BIT_WIDTH := 28

run : $(BINDIR)/$(TARGET)
	$(BINDIR)/$(TARGET) $(TOTAL_IMAGES) $(INTERLEAVED) $(BIT_WIDTH)

$(BINDIR)/$(TARGET) : $(OBJS) | $(BINDIR)
	@echo "$(YELLOW)Linking target: $@$(NO_COLOR)"
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o : %.c | $(OBJDIR)
	@echo "$(YELLOW)Compiling source: $@$(NO_COLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR) :
	mkdir $@

$(BINDIR) :
	mkdir $@

clean :
	-rm -rf $(OBJDIR)
	-rm -rf $(BINDIR)

purge : clean
	-rm -rf data
