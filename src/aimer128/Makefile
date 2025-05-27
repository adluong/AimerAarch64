# SPDX-License-Identifier: MIT

CC = gcc
AR = ar
CFLAGS = -O3 -Wall -Wextra -march=native -fomit-frame-pointer -fPIC -I../../common -I.
LDFLAGS = -lrt

# Library name
LIB_STATIC = libkucp_aimer.a
LIB_SHARED = libkucp_aimer.so

# Object files for the library
LIB_OBJS = kucp_aimer_wrapper.o \
           aim2_128s.o aim2_128f.o \
           sign_128s.o sign_128f.o \
           field_128s.o field_128f.o \
           hash_128s.o hash_128f.o \
           tree_128s.o tree_128f.o \
           __asm_field_128s.o __asm_field_128f.o \
           aes.o fips202.o rng.o

# Test program
TEST_PROG = test_kucp_aimer

.PHONY: all clean test lib

all: lib $(TEST_PROG)

lib: $(LIB_STATIC) $(LIB_SHARED)

# Make the preprocessing script executable
preprocess_asm.sh:
	chmod +x preprocess_asm.sh

# Create variant-specific assembly files
__asm_field_128s.S: __asm_field.S preprocess_asm.sh
	./preprocess_asm.sh 128s $@

__asm_field_128f.S: __asm_field.S preprocess_asm.sh
	./preprocess_asm.sh 128f $@

# Create variant-specific source files
aim2_128s.c: aim2.c
	echo '/* Auto-generated file for AIMER-128s */' > $@
	echo '#define GF_exp_invmer_e_1_2 GF_exp_invmer_e_1_2_128s' >> $@
	echo '#define GF_exp_mer_e_star GF_exp_mer_e_star_128s' >> $@
	echo '#define generate_matrices_L_and_U generate_matrices_L_and_U_128s' >> $@
	echo '#define generate_matrix_LU generate_matrix_LU_128s' >> $@
	echo '#include "aim2.c"' >> $@

aim2_128f.c: aim2.c
	echo '/* Auto-generated file for AIMER-128f */' > $@
	echo '#define GF_exp_invmer_e_1_2 GF_exp_invmer_e_1_2_128f' >> $@
	echo '#define GF_exp_mer_e_star GF_exp_mer_e_star_128f' >> $@
	echo '#define generate_matrices_L_and_U generate_matrices_L_and_U_128f' >> $@
	echo '#define generate_matrix_LU generate_matrix_LU_128f' >> $@
	echo '#include "aim2.c"' >> $@

# Wrapper object
kucp_aimer_wrapper.o: kucp_aimer_wrapper.c kucp_aimer_api.h
	$(CC) $(CFLAGS) -c $< -o $@

# Build object files for each variant
aim2_128s.o: aim2_128s.c aim2.h field.h params.h
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s -c $< -o $@

aim2_128f.o: aim2_128f.c aim2.h field.h params.h
	$(CC) $(CFLAGS) -DAIMER128F -Iaimer128f -c $< -o $@

sign_128s.o: sign.c sign.h aim2.h field.h hash.h params.h tree.h
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s -c $< -o $@

sign_128f.o: sign.c sign.h aim2.h field.h hash.h params.h tree.h
	$(CC) $(CFLAGS) -DAIMER128F -Iaimer128f -c $< -o $@

field_128s.o: field.c field.h params.h
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s -c $< -o $@

field_128f.o: field.c field.h params.h
	$(CC) $(CFLAGS) -DAIMER128F -Iaimer128f -c $< -o $@

hash_128s.o: hash.c hash.h params.h
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s -c $< -o $@

hash_128f.o: hash.c hash.h params.h
	$(CC) $(CFLAGS) -DAIMER128F -Iaimer128f -c $< -o $@

# Tree functions use wrapper files
tree_128s.o: tree_128s.c tree.h
	$(CC) $(CFLAGS) -DAIMER128S -Iaimer128s -c $< -o $@

tree_128f.o: tree_128f.c tree.h
	$(CC) $(CFLAGS) -DAIMER128F -Iaimer128f -c $< -o $@

__asm_field_128s.o: __asm_field_128s.S
	$(CC) $(CFLAGS) -c $< -o $@

__asm_field_128f.o: __asm_field_128f.S
	$(CC) $(CFLAGS) -c $< -o $@

# Common object files
aes.o: ../../common/aes.c
	$(CC) $(CFLAGS) -c $< -o $@

fips202.o: ../../common/fips202.c
	$(CC) $(CFLAGS) -c $< -o $@

rng.o: ../../common/rng.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build static library
$(LIB_STATIC): $(LIB_OBJS)
	$(AR) rcs $@ $^

# Build shared library
$(LIB_SHARED): $(LIB_OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

# Build test program
$(TEST_PROG): test_kucp_aimer.c $(LIB_STATIC)
	$(CC) $(CFLAGS) $< -L. -lkucp_aimer -o $@ $(LDFLAGS)

# Run test
test: $(TEST_PROG)
	LD_LIBRARY_PATH=. ./$(TEST_PROG)

# Install library (optional)
install: $(LIB_STATIC) $(LIB_SHARED) kucp_aimer_api.h
	install -d $(DESTDIR)/usr/local/lib
	install -d $(DESTDIR)/usr/local/include
	install -m 644 $(LIB_STATIC) $(DESTDIR)/usr/local/lib/
	install -m 755 $(LIB_SHARED) $(DESTDIR)/usr/local/lib/
	install -m 644 kucp_aimer_api.h $(DESTDIR)/usr/local/include/

# Clean build artifacts
clean:
	rm -f $(LIB_STATIC) $(LIB_SHARED) $(TEST_PROG)
	rm -f __asm_field_128s.S __asm_field_128f.S
	rm -f aim2_128s.c aim2_128f.c tree_128s.c tree_128f.c
	rm -f *.o
	rm -f PQCsignKAT_*.rsp PQCsignKAT_*.req
