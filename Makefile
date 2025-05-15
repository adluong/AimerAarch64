CC = gcc
CFLAGS = -O3 -march=native -fomit-frame-pointer -Wall
# Add -fPIC for shared library position-independent code
# Add -DNDEBUG for release builds

# Common sources - add missing tree.c and aes.c
COMMON_SRC = common/rng.c common/cpucycles.c common/fips202.c common/tree.c common/aes.c

# AIMER variants
VARIANTS = aimer128f aimer128s aimer192f aimer192s aimer256f aimer256s

# Build all variants as separate libraries to avoid symbol conflicts
.PHONY: all
all: test_main

# Object files for common code
COMMON_OBJS = $(COMMON_SRC:.c=.o)

# Compile common source files
$(COMMON_OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main program
main.o: main.c api.h
	$(CC) $(CFLAGS) -c $< -o $@

# For each variant, compile with unique namespaces to avoid symbol conflicts
define compile_variant
src/$(1)/%.o: src/$(1)/%.c
	$(CC) $(CFLAGS) -I. -Isrc/$(1) -DAIMER_VARIANT=$(1) -c $< -o $@

src/$(1)/__asm_field.o: src/$(1)/__asm_field.S
	$(CC) $(CFLAGS) -c $< -o $@

src/$(1)_objs: $(patsubst %.c,%.o,$(wildcard src/$(1)/*.c)) src/$(1)/__asm_field.o
	@echo "Building variant $(1) complete"
endef

# Generate compilation rules for each variant
$(foreach variant,$(VARIANTS),$(eval $(call compile_variant,$(variant))))

# Build the test program
test_main: main.o $(COMMON_OBJS) $(foreach variant,$(VARIANTS),src/$(variant)_objs)
	$(CC) $(CFLAGS) main.o $(COMMON_OBJS) $(foreach variant,$(VARIANTS),$(patsubst %.c,%.o,$(wildcard src/$(variant)/*.c)) src/$(variant)/__asm_field.o) -o main

# Clean rule
clean:
	rm -f main main.o $(COMMON_OBJS)
	for variant in $(VARIANTS); do \
		rm -f src/$variant/*.o; \
	done

# Alternative approach: build each variant as a separate library
# This would be better for a production library setup
lib: $(foreach variant,$(VARIANTS),lib$(variant).a)

lib%.a: src/%_objs $(COMMON_OBJS)
	ar rcs $@ $(patsubst %.c,%.o,$(wildcard src/$*/*.c)) src/$*/__asm_field.o $(COMMON_OBJS)