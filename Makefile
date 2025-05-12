CC = gcc
CFLAGS = -O3 -march=native -fomit-frame-pointer

VARIANTS = src/aimer128f src/aimer128s src/aimer192f src/aimer192s src/aimer256f src/aimer256s

ALL_TESTAIM2 = $(patsubst %, %/tests/test_aim2, $(VARIANTS))
ALL_TESTSIGN = $(patsubst %, %/tests/test_sign, $(VARIANTS))
ALL_TESTSPEED = $(patsubst %, %/tests/test_speed, $(VARIANTS))
ALL_KAT  = $(patsubst %, %/PQCgenKAT_sign, $(VARIANTS))
#ALL_TEST = $(patsubst %, %/test, $(VARIANTS))

all: $(ALL_TESTAIM2) $(ALL_TESTSIGN) $(ALL_TESTSPEED) $(ALL_KAT)

$(ALL_TESTAIM2): %/tests/test_aim2: %/tests/test_aim2.c %/aim2.c %/field.c %/__asm_field.S %/hash.c common/fips202.c
	$(CC) $(CFLAGS) -I$* $^ -o $@

$(ALL_TESTSIGN): %/tests/test_sign: %/tests/test_sign.c %/aim2.c %/field.c %/__asm_field.S %/hash.c %/sign.c %/tree.c common/aes.c common/fips202.c common/rng.c
	$(CC) $(CFLAGS) -I$* $^ -o $@

$(ALL_TESTSPEED): %/tests/test_speed: %/tests/test_speed.c %/aim2.c %/field.c %/__asm_field.S %/hash.c %/sign.c %/tree.c common/aes.c common/speed_print.c common/cpucycles.c common/fips202.c common/rng.c
	$(CC) $(CFLAGS) -I$* $^ -o $@

$(ALL_KAT): %/PQCgenKAT_sign: common/PQCgenKAT_sign.c %/aim2.c %/field.c %/__asm_field.S %/hash.c %/sign.c %/tree.c common/aes.c common/fips202.c common/rng.c
	$(CC) $(CFLAGS) -I$* $^ -o $@

#$(ALL_TEST): %/test: common/test.c %/aim2.c %/field.c %/__asm_field.S %/hash.c %/sign.c %/tree.c common/aes.c common/fips202.c common/rng.c
#	$(CC) $(CFLAGS) -I$* $^ -o $@
clean:
	rm -f $(ALL_TESTAIM2) $(ALL_TESTSIGN) $(ALL_TESTSPEED) $(ALL_KAT)
	for variant in $(VARIANTS); do \
		rm -f $$variant/PQCsignKAT_*.rsp $$variant/PQCsignKAT_*.req; \
	done

.PHONY: all clean
