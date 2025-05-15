CC = gcc
CFLAGS = -O3 -march=native -fomit-frame-pointer

VARIANTS = src/aimer128f src/aimer128s src/aimer192f src/aimer192s src/aimer256f src/aimer256s
OBJDIR = obj

# Shared source files and objects
SHARED_SRC = common/fips202.c common/aes.c common/rng.c common/tree.c common/speed_print.c common/cpucycles.c
SHARED_OBJ = $(patsubst common/%.c, $(OBJDIR)/common/%.o, $(SHARED_SRC))

# Test executables
ALL_TESTAIM2 = $(patsubst %, %/tests/test_aim2, $(VARIANTS))
ALL_TESTSIGN = $(patsubst %, %/tests/test_sign, $(VARIANTS))
ALL_TESTSPEED = $(patsubst %, %/tests/test_speed, $(VARIANTS))
ALL_KAT = $(patsubst %, %/PQCgenKAT_sign, $(VARIANTS))

all: $(ALL_TESTAIM2) $(ALL_TESTSIGN) $(ALL_TESTSPEED) $(ALL_KAT)

# Variant-specific object files
define variant_obj
$(OBJDIR)/$*/aim2.o $(OBJDIR)/$*/field.o $(OBJDIR)/$*/hash.o $(OBJDIR)/$*/sign.o $(OBJDIR)/$*/__asm_field.o
endef

# Build rules for test executables
$(ALL_TESTAIM2): %/tests/test_aim2: $(OBJDIR)/%/tests/test_aim2.o $(filter-out $(OBJDIR)/%/sign.o, $(call variant_obj,%)) $(OBJDIR)/common/fips202.o
	$(CC) $(CFLAGS) $^ -o $@

$(ALL_TESTSIGN): %/tests/test_sign: $(OBJDIR)/%/tests/test_sign.o $(call variant_obj,%) $(OBJDIR)/common/fips202.o $(OBJDIR)/common/aes.o $(OBJDIR)/common/rng.o $(OBJDIR)/common/tree.o
	$(CC) $(CFLAGS) $^ -o $@

$(ALL_TESTSPEED): %/tests/test_speed: $(OBJDIR)/%/tests/test_speed.o $(call variant_obj,%) $(OBJDIR)/common/fips202.o $(OBJDIR)/common/aes.o $(OBJDIR)/common/rng.o $(OBJDIR)/common/tree.o $(OBJDIR)/common/speed_print.o $(OBJDIR)/common/cpucycles.o
	$(CC) $(CFLAGS) $^ -o $@

$(ALL_KAT): %/PQCgenKAT_sign: $(OBJDIR)/common/PQCgenKAT_sign.o $(call variant_obj,%) $(OBJDIR)/common/fips202.o $(OBJDIR)/common/aes.o $(OBJDIR)/common/rng.o $(OBJDIR)/common/tree.o
	$(CC) $(CFLAGS) $^ -o $@

# Compilation rules
$(OBJDIR)/%/aim2.o: %/aim2.c | $(OBJDIR)/%
	$(CC) $(CFLAGS) -I$* -c $< -o $@

$(OBJDIR)/%/field.o: %/field.c | $(OBJDIR)/%
	$(CC) $(CFLAGS) -I$* -c $< -o $@

$(OBJDIR)/%/hash.o: %/hash.c | $(OBJDIR)/%
	$(CC) $(CFLAGS) -I$* -c $< -o $@

$(OBJDIR)/%/sign.o: %/sign.c | $(OBJDIR)/%
	$(CC) $(CFLAGS) -I$* -c $< -o $@

$(OBJDIR)/%/tests/test_aim2.o: %/tests/test_aim2.c | $(OBJDIR)/%/tests
	$(CC) $(CFLAGS) -I$* -c $< -o $@

$(OBJDIR)/%/tests/test_sign.o: %/tests/test_sign.c | $(OBJDIR)/%/tests
	$(CC) $(CFLAGS) -I$* -c $< -o $@

$(OBJDIR)/%/tests/test_speed.o: %/tests/test_speed.c | $(OBJDIR)/%/tests
	$(CC) $(CFLAGS) -I$* -c $< -o $@

$(OBJDIR)/common/PQCgenKAT_sign.o: common/PQCgenKAT_sign.c | $(OBJDIR)/common
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/common/%.o: common/%.c | $(OBJDIR)/common
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%/__asm_field.o: %/__asm_field.S | $(OBJDIR)/%
	$(CC) $(CFLAGS) -c $< -o $@

# Directory creation
$(OBJDIR)/%/tests $(OBJDIR)/% $(OBJDIR)/common:
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(ALL_TESTAIM2) $(ALL_TESTSIGN) $(ALL_TESTSPEED) $(ALL_KAT)
	for variant in $(VARIANTS); do rm -f $$variant/PQCsignKAT_*.rsp $$variant/PQCsignKAT_*.req; done

.PHONY: all clean