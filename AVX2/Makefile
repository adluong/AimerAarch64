CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wpointer-arith -mavx2 -mpopcnt -march=native -mtune=native -O3 -fPIC

OBJDIR = obj
LIBNAME = libkucp_aimer_avx2.so

PARAMSETS = \
  aimer128/aimer128f \
  aimer128/aimer128s \
  aimer192/aimer192f \
  aimer192/aimer192s \
  aimer256/aimer256f \
  aimer256/aimer256s

COMMON_FILES = aim2 field hash sign tree
COMMON_DIRS = aimer128 aimer192 aimer256

KECCAK_SOURCES = \
  common/keccak_avx2/fips202.c \
  common/keccak_avx2/KeccakP-1600-AVX2.s \
  common/keccak4x/KeccakP-1600-times4-SIMD256.c

COMMON_SOURCES = common/aes.c common/rng.c

ALL_COMMON_OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(filter %.c,$(KECCAK_SOURCES) $(COMMON_SOURCES))) \
                  $(patsubst %.s, $(OBJDIR)/%.o, $(filter %.s,$(KECCAK_SOURCES)))

ALL_AIMER_OBJS = $(foreach file,$(COMMON_FILES),$(foreach param,$(PARAMSETS),\
                  $(OBJDIR)/$(file)_$(subst /,_,$(param)).o))

.PHONY: all clean

all: $(LIBNAME) main

$(LIBNAME): $(ALL_COMMON_OBJS) $(ALL_AIMER_OBJS)
	$(CC) -shared -o $@ $^

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

define compile_param_template
$(OBJDIR)/$(1)_$(subst /,_,$(2)).o: $(3)/$(1).c $(2)/params.h
	mkdir -p $$(dir $$@)
	$$(CC) $$(CFLAGS) -I$(2) -c $$< -o $$@
endef

$(foreach file,$(COMMON_FILES),$(foreach param,$(PARAMSETS),\
  $(eval $(call compile_param_template,$(file),$(param),$(word 1,$(subst /, ,$(param)))))))

main: main.c $(LIBNAME)
	$(CC) $(CFLAGS) -o $@ main.c -L. -lkucp_aimer_avx2 -Wl,-rpath=.

clean:
	rm -rf $(OBJDIR) *.so main