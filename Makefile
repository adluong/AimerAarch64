###############################################
# Root Makefile for aarch64 KUCP-AIMER project  #
###############################################

# Compiler and flags
CC      := gcc
CFLAGS  := -O3 -march=armv8-a -I. -Icommon
AS      := gcc
ASFLAGS := -O3 -march=armv8-a

# Directories
COMMON_DIR := common
SRC_DIR    := src

# Sources
COMMON_SRCS := $(wildcard $(COMMON_DIR)/*.c)
AIMER_C_SRCS := $(wildcard $(SRC_DIR)/*/*.c)
AIMER_S_SRCS := $(wildcard $(SRC_DIR)/*/*.S)

# Object files
COMMON_OBJS := $(patsubst %.c, %.o, $(COMMON_SRCS))
AIMER_C_OBJS := $(patsubst %.c, %.o, $(AIMER_C_SRCS))
AIMER_S_OBJS := $(patsubst %.S, %.o, $(AIMER_S_SRCS))
ALL_OBJS    := $(COMMON_OBJS) $(AIMER_C_OBJS) $(AIMER_S_OBJS)

# Main executable
TARGET := main

.PHONY: all clean

all: $(TARGET)

# Link main with all objects
$(TARGET): main.c $(ALL_OBJS)
	$(CC) $(CFLAGS) -o $@ main.c $(ALL_OBJS)

# Compile common C sources
$(COMMON_OBJS): %.o: %.c
	$(CC) $(CFLAGS) -Icommon -c $< -o $@

# Compile variant-specific C sources
$(AIMER_C_OBJS): %.o: %.c
	$(CC) $(CFLAGS) -Icommon -I$(dir $<) -c $< -o $@

# Assemble variant-specific .S sources
$(AIMER_S_OBJS): %.o: %.S
	$(AS) $(ASFLAGS) -Icommon -I$(dir $<) -c $< -o $@

clean:
	rm -f $(ALL_OBJS) $(TARGET)
