/*-----------------------------------------------------------------------
 * main.c – very small regression test for libkucpaimer
 *---------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "kucp_aimer_api.h"

/* A single message we will sign and verify */
static const unsigned char MESSAGE[] = "Hello, KUCP-Aimer!";
static const unsigned long long MLEN   = sizeof(MESSAGE) - 1;   /* exclude NUL */

/* --- helper: stop a sub-test on error --------------------------------*/
#define CHECK(expr, stage)                       \
    do {                                         \
        if ((expr) != 0) {                       \
            fprintf(stderr, "%s failed\n", stage); \
            return 0;                            \
        }                                        \
    } while (0)

/* ---------- aimer128f -------------------------------------------------*/
static int test_128f(void)
{
    puts("Testing aimer128f…");

    unsigned char pk[kucp_aimer128f_PUBLICKEYBYTES];
    unsigned char sk[kucp_aimer128f_SECRETKEYBYTES];

    CHECK(kucp_aimer128f_aarch64_sign_keypair(pk, sk), "keypair");

    unsigned char sm[MLEN + kucp_aimer128f_BYTES];
    unsigned long long smlen = 0;
    CHECK(kucp_aimer128f_aarch64_sign(sm, &smlen, MESSAGE, MLEN, sk), "sign");

    unsigned char m2[MLEN + kucp_aimer128f_BYTES];
    unsigned long long m2len = 0;
    CHECK(kucp_aimer128f_aarch64_sign_open(m2, &m2len, sm, smlen, pk), "verify");

    if (m2len != MLEN || memcmp(MESSAGE, m2, MLEN) != 0) {
        fputs("verification returned wrong message\n", stderr);
        return 0;
    }
    puts("  ✓ 128f OK");
    return 1;
}

/* ---------- aimer128s -------------------------------------------------*/
static int test_128s(void)
{
    puts("Testing aimer128s…");

    unsigned char pk[kucp_aimer128s_PUBLICKEYBYTES];
    unsigned char sk[kucp_aimer128s_SECRETKEYBYTES];

    CHECK(kucp_aimer128s_aarch64_sign_keypair(pk, sk), "keypair");

    unsigned char sm[MLEN + kucp_aimer128s_BYTES];
    unsigned long long smlen = 0;
    CHECK(kucp_aimer128s_aarch64_sign(sm, &smlen, MESSAGE, MLEN, sk), "sign");

    unsigned char m2[MLEN + kucp_aimer128s_BYTES];
    unsigned long long m2len = 0;
    CHECK(kucp_aimer128s_aarch64_sign_open(m2, &m2len, sm, smlen, pk), "verify");

    if (m2len != MLEN || memcmp(MESSAGE, m2, MLEN) != 0) {
        fputs("verification returned wrong message\n", stderr);
        return 0;
    }
    puts("  ✓ 128s OK");
    return 1;
}

/* ---------------------------------------------------------------------*/
int main(void)
{
    int ok = 1;
    ok &= test_128f();
    ok &= test_128s();

    if (ok) {
        puts("\nAll basic signer tests succeeded.");
        return 0;
    }
    puts("\nAt least one test FAILED.");
    return 1;
}

