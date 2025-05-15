
CC = gcc
CFLAGS = -O3 -march=native -fomit-frame-pointer -Wall

# Common sources
COMMON_SRC = common/rng.c common/cpucycles.c common/fips202.c

# AIMER variants
VARIANTS = aimer128f aimer128s aimer192f aimer192s aimer256f aimer256s

# Define object files for each variant
define variant_objs
$(1)_OBJS = src/$(1)/aim2.o src/$(1)/field.o src/$(1)/__asm_field.o src/$(1)/hash.o src/$(1)/sign.o
endef

# Generate object file variables for each variant
$(foreach variant,$(VARIANTS),$(eval $(call variant_objs,$(variant))))

# All object files
ALL_OBJS = $(foreach variant,$(VARIANTS),$($(variant)_OBJS)) $(COMMON_SRC:.c=.o)

# Target
all: main

# Pattern rule for compiling .c files
%.o: %.c
	$(CC) $(CFLAGS) -I./src/$(@D:common=common) -c $< -o $@

# Pattern rule for compiling .S files
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

# Build main executable
main: main.o $(ALL_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean rule
clean:
	rm -f main main.o $(ALL_OBJS)

.PHONY: all clean
