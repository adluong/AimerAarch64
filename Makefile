CC = gcc
CFLAGS = -O3 -march=native -fomit-frame-pointer

VARIANTS = src/aimer128f src/aimer128s src/aimer192f src/aimer192s src/aimer256f src/aimer256s

ALL_TESTAIM2 = $(patsubst %, %/tests/test_aim2, $(VARIANTS))
ALL_TESTSIGN = $(patsubst %, %/tests/test_sign, $(VARIANTS))
ALL_TESTSPEED = $(patsubst %, %/tests/test_speed, $(VARIANTS))
ALL_KAT  = $(patsubst %, %/PQCgenKAT_sign, $(VARIANTS))

OBJDIR = obj/common

all: $(ALL_TESTAIM2) $(ALL_TESTSIGN) $(ALL_TESTSPEED) $(ALL_KAT)

# Linking rules
$(ALL_TESTAIM2): %/tests/test_aim2: $(OBJDIR)/$(notdir $*_test_aim2).o $(OBJDIR)/$(notdir $*_aim2).o $(OBJDIR)/$(notdir $*_field).o $(OBJDIR)/$(notdir $*___asm_field).o $(OBJDIR)/$(notdir $*_hash).o $(OBJDIR)/fips202.o
	$(CC) $(CFLAGS) $^ -o $@

$(ALL_TESTSIGN): %/tests/test_sign: $(OBJDIR)/$(notdir $*_test_sign).o $(OBJDIR)/$(notdir $*_aim2).o $(OBJDIR)/$(notdir $*_field).o $(OBJDIR)/$(notdir $*___asm_field).o $(OBJDIR)/$(notdir $*_hash).o $(OBJDIR)/$(notdir $*_sign).o $(OBJDIR)/tree.o $(OBJDIR)/aes.o $(OBJDIR)/fips202.o $(OBJDIR)/rng.o
	$(CC) $(CFLAGS) $^ -o $@

$(ALL_TESTSPEED): %/tests/test_speed: $(OBJDIR)/$(notdir $*_test_speed).o $(OBJDIR)/$(notdir $*_aim2).o $(OBJDIR)/$(notdir $*_field).o $(OBJDIR)/$(notdir $*___asm_field).o $(OBJDIR)/$(notdir $*_hash).o $(OBJDIR)/$(notdir $*_sign).o $(OBJDIR)/tree.o $(OBJDIR)/aes.o $(OBJDIR)/speed_print.o $(OBJDIR)/cpucycles.o $(OBJDIR)/fips202.o $(OBJDIR)/rng.o
	$(CC) $(CFLAGS) $^ -o $@

$(ALL_KAT): %/PQCgenKAT_sign: $(OBJDIR)/$(notdir $*_PQCgenKAT_sign).o $(OBJDIR)/$(notdir $*_aim2).o $(OBJDIR)/$(notdir $*_field).o $(OBJDIR)/$(notdir $*___asm_field).o $(OBJDIR)/$(notdir $*_hash).o $(OBJDIR)/$(notdir $*_sign).o $(OBJDIR)/tree.o $(OBJDIR)/aes.o $(OBJDIR)/fips202.o $(OBJDIR)/rng.o
	$(CC) $(CFLAGS) $^ -o $@

# Compilation rules for variant-specific source files
$(OBJDIR)/%_aim2.o: src/%/aim2.c
	$(CC) $(CFLAGS) -I src/$* -c $< -o $@

$(OBJDIR)/%_field.o: src/%/field.c
	$(CC) $(CFLAGS) -I src/$* -c $< -o $@

$(OBJDIR)/%_hash.o: src/%/hash.c
	$(CC) $(CFLAGS) -I src/$* -c $< -o $@

$(OBJDIR)/%_sign.o: src/%/sign.c
	$(CC) $(CFLAGS) -I src/$* -c $< -o $@

$(OBJDIR)/%___asm_field.o: src/%/__asm_field.S
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation rules for test source files
$(OBJDIR)/%_test_aim2.o: src/%/tests/test_aim2.c
	$(CC) $(CFLAGS) -I src/$* -c $< -o $@

$(OBJDIR)/%_test_sign.o: src/%/tests/test_sign.c
	$(CC) $(CFLAGS) -I src/$* -c $< -o $@

$(OBJDIR)/%_test_speed.o: src/%/tests/test_speed.c
	$(CC) $(CFLAGS) -I src/$* -c $< -o $@

$(OBJDIR)/%_PQCgenKAT_sign.o: common/PQCgenKAT_sign.c
	$(CC) $(CFLAGS) -I src/$* -c $< -o $@

# Compilation rules for shared source files
$(OBJDIR)/fips202.o: common/fips202.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/aes.o: common/aes.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/rng.o: common/rng.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/tree.o: common/tree.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/speed_print.o: common/speed_print.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/cpucycles.o: common/cpucycles.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(ALL_TESTAIM2) $(ALL_TESTSIGN) $(ALL_TESTSPEED) $(ALL_KAT)
	rm -rf $(OBJDIR)
	for variant in $(VARIANTS); do \
		rm -f $$variant/PQCsignKAT_*.rsp $$variant/PQCsignKAT_*.req; \
	done

.PHONY: all clean
