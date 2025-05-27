CC ?= gcc
AR ?= ar
CFLAGS ?= -O3 -march=native -std=c11 -Wall -Wextra -fPIC -I. -I../../common
LDFLAGS ?= -Wl,--allow-multiple-definition

BUILD    := build
DIR128F  := $(BUILD)/128f
DIR128S  := $(BUILD)/128s

CORE_C   = aim2.c field.c hash.c sign.c
ASM128F  = __asm_field_128f.S
ASM128S  = __asm_field_128s.S
WRAP     = kucp_aimer_warpper.c

ONCE_C   = ../../common/fips202.c ../../common/rng.c ../../common/aes.c
PERVAR_C = ../../common/tree.c   ../../common/hash.c

OBJ128F  = $(patsubst %.c,$(DIR128F)/%.o,$(CORE_C)) \
           $(DIR128F)/$(ASM128F:.S=.o)              \
           $(addprefix $(DIR128F)/,$(notdir $(PERVAR_C:.c=.o)))

OBJ128S  = $(patsubst %.c,$(DIR128S)/%.o,$(CORE_C)) \
           $(DIR128S)/$(ASM128S:.S=.o)              \
           $(addprefix $(DIR128S)/,$(notdir $(PERVAR_C:.c=.o)))

OBJONCE  = $(addprefix $(BUILD)/,$(notdir $(ONCE_C:.c=.o)))
OBJWRAP  = $(BUILD)/$(notdir $(WRAP:.c=.o))

OBJECTS  = $(OBJ128F) $(OBJ128S) $(OBJONCE) $(OBJWRAP)

LIB_A  = libkucpaimer.a
LIB_SO = libkucpaimer.so

.PHONY: all clean
all: $(LIB_A) $(LIB_SO)

$(BUILD) $(DIR128F) $(DIR128S):
	mkdir -p $@

$(DIR128F)/%.o: %.c | $(DIR128F)
	$(CC) $(CFLAGS) -Iaimer128f -c $< -o $@

$(DIR128S)/%.o: %.c | $(DIR128S)
	$(CC) $(CFLAGS) -Iaimer128s -c $< -o $@

$(DIR128F)/%.o: %.S | $(DIR128F)
	$(CC) $(CFLAGS) -Iaimer128f -c $< -o $@

$(DIR128S)/%.o: %.S | $(DIR128S)
	$(CC) $(CFLAGS) -Iaimer128s -c $< -o $@

$(DIR128F)/%.o: ../../common/%.c | $(DIR128F)
	$(CC) $(CFLAGS) -Iaimer128f -c $< -o $@

$(DIR128S)/%.o: ../../common/%.c | $(DIR128S)
	$(CC) $(CFLAGS) -Iaimer128s -c $< -o $@

$(BUILD)/%.o: ../../common/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJWRAP): $(WRAP) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_A): $(OBJECTS)
	$(AR) rcs $@ $^

$(LIB_SO): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $^

clean:
	rm -rf $(BUILD) $(LIB_A) $(LIB_SO)

