# SPDX-License-Identifier: MIT

CC = gcc
CFLAGS = -O3 -Wall -Wextra -march=native -fomit-frame-pointer -I../../common
LDFLAGS = -lrt

# Common source files (without assembly)
COMMON_C_SRCS = aim2.c field.c hash.c sign.c \
                ../../common/tree.c ../../common/aes.c \
                ../../common/fips202.c ../../common/rng.c

# Test executables
TEST_AIM2 = test/test_aim2
TEST_SIGN = test/test_sign
TEST_SPEED = test/test_speed
TEST_192S = test_aimer192s
TEST_192F = test_aimer192f

# NIST KAT generator
KAT_192S = PQCgenKAT_sign_aimer192s
KAT_192F = PQCgenKAT_sign_aimer192f

.PHONY: all clean test test_all

all: $(TEST_192S) $(TEST_192F)

# Make the preprocessing script executable
preprocess_asm.sh:
	chmod +x preprocess_asm.sh

# Create variant-specific assembly files
__asm_field_192s.S: __asm_field.S preprocess_asm.sh
	./preprocess_asm.sh 192s $@

__asm_field_192f.S: __asm_field.S preprocess_asm.sh
	./preprocess_asm.sh 192f $@

# Build AIMER-192s test
$(TEST_192S): main.c $(COMMON_C_SRCS) __asm_field_192s.S
	$(CC) $(CFLAGS) -DAIMER192S -Iaimer192s $^ -o $@ $(LDFLAGS)

# Build AIMER-192f test  
$(TEST_192F): main.c $(COMMON_C_SRCS) __asm_field_192f.S
	$(CC) $(CFLAGS) -DAIMER192F -Iaimer192f $^ -o $@ $(LDFLAGS)

# Build individual tests
$(TEST_AIM2): test/test_aim2.c aim2.c field.c __asm_field_192s.S hash.c ../../common/fips202.c
	$(CC) $(CFLAGS) -DAIMER192S -Iaimer192s $^ -o $@

$(TEST_SIGN): test/test_sign.c $(COMMON_C_SRCS) __asm_field_192s.S
	$(CC) $(CFLAGS) -DAIMER192S -Iaimer192s $^ -o $@ $(LDFLAGS)

$(TEST_SPEED): test/test_speed.c $(COMMON_C_SRCS) __asm_field_192s.S ../../common/speed_print.c ../../common/cpucycles.c
	$(CC) $(CFLAGS) -DAIMER192S -Iaimer192s $^ -o $@ $(LDFLAGS)

# Build NIST KAT generators
$(KAT_192S): ../../common/PQCgenKAT_sign.c $(COMMON_C_SRCS) __asm_field_192s.S
	$(CC) $(CFLAGS) -DAIMER192S -Iaimer192s $^ -o $@

$(KAT_192F): ../../common/PQCgenKAT_sign.c $(COMMON_C_SRCS) __asm_field_192f.S
	$(CC) $(CFLAGS) -DAIMER192F -Iaimer192f $^ -o $@

# Run tests
test: $(TEST_192S) $(TEST_192F)
	@echo "Testing AIMER-192s..."
	./$(TEST_192S)
	@echo "\nTesting AIMER-192f..."
	./$(TEST_192F)

test_all: test $(TEST_AIM2) $(TEST_SIGN) $(TEST_SPEED)
	@echo "\nRunning AIM2 test..."
	./$(TEST_AIM2)
	@echo "\nRunning signature test..."
	./$(TEST_SIGN)
	@echo "\nRunning speed test..."
	./$(TEST_SPEED)

# Generate KAT files
kat: $(KAT_192S) $(KAT_192F)
	./$(KAT_192S)
	./$(KAT_192F)

# Clean build artifacts
clean:
	rm -f $(TEST_192S) $(TEST_192F) $(TEST_AIM2) $(TEST_SIGN) $(TEST_SPEED)
	rm -f $(KAT_192S) $(KAT_192F)
	rm -f __asm_field_192s.S __asm_field_192f.S
	rm -f *.o test/*.o
	rm -f PQCsignKAT_*.rsp PQCsignKAT_*.req
