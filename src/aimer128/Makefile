CC ?= gcc
AR ?= ar
CFLAGS ?= -O3 -march=native -std=c11 -Wall -Wextra -fPIC -I. -I../../common
LDFLAGS ?= -Wl,--allow-multiple-definition

SRC_COMMON = aim2.c field.c hash.c sign.c
SRC_COMMON_EXTRA = ../../common/fips202.c ../../common/rng.c ../../common/tree.c ../../common/aes.c ../../hash.c 
ASM_128f_SRC = __asm_field_128f.S
ASM_128s_SRC = __asm_field_128s.S
WRAP_SRC = kucp_aimer_warpper.c

BUILD = build
OBJ_128f = $(addprefix $(BUILD)/128f/, $(SRC_COMMON:.c=.o) $(ASM_128f_SRC:.S=.o))
OBJ_128s = $(addprefix $(BUILD)/128s/, $(SRC_COMMON:.c=.o) $(ASM_128s_SRC:.S=.o))
OBJ_EXTRA = $(addprefix $(BUILD)/, $(notdir $(SRC_COMMON_EXTRA:.c=.o)))
OBJ_WRAP = $(BUILD)/$(WRAP_SRC:.c=.o)
OBJECTS = $(OBJ_128f) $(OBJ_128s) $(OBJ_EXTRA) $(OBJ_WRAP)

LIB_STATIC = libkucpaimer.a
LIB_SHARED = libkucpaimer.so

.PHONY: all clean
all: $(LIB_STATIC) $(LIB_SHARED)

$(BUILD) $(BUILD)/128f $(BUILD)/128s:
	mkdir -p $@

$(BUILD)/128f/%.o: %.c | $(BUILD)/128f
	$(CC) $(CFLAGS) -Iaimer128f -c $< -o $@

$(BUILD)/128s/%.o: %.c | $(BUILD)/128s
	$(CC) $(CFLAGS) -Iaimer128s -c $< -o $@

$(BUILD)/128f/%.o: %.S | $(BUILD)/128f
	$(CC) $(CFLAGS) -Iaimer128f -c $< -o $@

$(BUILD)/128s/%.o: %.S | $(BUILD)/128s
	$(CC) $(CFLAGS) -Iaimer128s -c $< -o $@

$(BUILD)/%.o: ../../common/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_WRAP): $(WRAP_SRC) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_STATIC): $(OBJECTS)
	$(AR) rcs $@ $^

$(LIB_SHARED): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $^

clean:
	rm -rf $(BUILD) $(LIB_STATIC) $(LIB_SHARED)

