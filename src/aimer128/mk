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
TEST_128S = test_aimer128s
TEST_128F = test_aimer128f

# NIST KAT generator
KAT_128S = PQCgenKAT_sign_aimer128s
KAT_128F = PQCgenKAT_sign_aimer128f

.PHONY: all clean test test_all

all: $(TEST_128S) $(TEST_128F)

# Make the preprocessing script executable
preprocess_asm.sh:
	chmod +x preprocess_asm.sh

# Create variant-specific assembly files
__asm_field_128s.S: __asm_field.S preprocess_asm.sh
	./preprocess_asm.sh 128s $@

__asm_field_128f.S: __asm_field.S preprocess_asm.sh
	./preprocess_asm.sh 128f $@

# Build AIMER-128s test
$(TEST_128S): main.c $(COMMON_C_SRCS) __asm_field_128s.S
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s $^ -o $@ $(LDFLAGS)

# Build AIMER-128f test  
$(TEST_128F): main.c $(COMMON_C_SRCS) __asm_field_128f.S
	$(CC) $(CFLAGS) -DAIMER128F -Iaimer128f $^ -o $@ $(LDFLAGS)

# Build individual tests
$(TEST_AIM2): test/test_aim2.c aim2.c field.c __asm_field_128s.S hash.c ../../common/fips202.c
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s $^ -o $@

$(TEST_SIGN): test/test_sign.c $(COMMON_C_SRCS) __asm_field_128s.S
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s $^ -o $@ $(LDFLAGS)

$(TEST_SPEED): test/test_speed.c $(COMMON_C_SRCS) __asm_field_128s.S ../../common/speed_print.c ../../common/cpucycles.c
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s $^ -o $@ $(LDFLAGS)

# Build NIST KAT generators
$(KAT_128S): ../../common/PQCgenKAT_sign.c $(COMMON_C_SRCS) __asm_field_128s.S
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s $^ -o $@

$(KAT_128F): ../../common/PQCgenKAT_sign.c $(COMMON_C_SRCS) __asm_field_128f.S
	$(CC) $(CFLAGS) -DAIMER128F -Iaimer128f $^ -o $@

# Run tests
test: $(TEST_128S) $(TEST_128F)
	@echo "Testing AIMER-128s..."
	./$(TEST_128S)
	@echo "\nTesting AIMER-128f..."
	./$(TEST_128F)

test_all: test $(TEST_AIM2) $(TEST_SIGN) $(TEST_SPEED)
	@echo "\nRunning AIM2 test..."
	./$(TEST_AIM2)
	@echo "\nRunning signature test..."
	./$(TEST_SIGN)
	@echo "\nRunning speed test..."
	./$(TEST_SPEED)

# Generate KAT files
kat: $(KAT_128S) $(KAT_128F)
	./$(KAT_128S)
	./$(KAT_128F)

# Clean build artifacts
clean:
	rm -f $(TEST_128S) $(TEST_128F) $(TEST_AIM2) $(TEST_SIGN) $(TEST_SPEED)
	rm -f $(KAT_128S) $(KAT_128F)
	rm -f __asm_field_128s.S __asm_field_128f.S
	rm -f *.o test/*.o
	rm -f PQCsignKAT_*.rsp PQCsignKAT_*.req
