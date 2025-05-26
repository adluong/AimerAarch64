# SPDX-License-Identifier: MIT

CC = gcc
CFLAGS += -I. -O3 -Wall -Wextra -march=native -fomit-frame-pointer
NISTFLAGS = -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-result

EXECUTABLE_KAT       = PQCgenKAT_sign

.PHONY: all
all:  $(EXECUTABLE_KAT)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(EXECUTABLE_KAT): ../../common/$(EXECUTABLE_KAT).c aim2.c field.c __asm_field.S hash.c sign.c ../../common/tree.c ../../common/aes.c ../../common/fips202.c ../../common/rng.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(wildcard *.o) $(EXECUTABLE_TESTAIM2) $(EXECUTABLE_TESTSIGN) $(EXECUTABLE_TESTSPEED) $(EXECUTABLE_KAT) PQCsignKAT_*.rsp PQCsignKAT_*.req
