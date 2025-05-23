#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "api.h"
#include "common/rng.h"
#include "cpucycles.h"

#define MLEN 59
#define TEST_LOOP 100

unsigned long long cyclegap = 0;

static void TEST_SIGN_AIMER128F(void)
{
    unsigned char m[MLEN + samsungsds_aimer128f_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer128f_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer128f_BYTES];
    unsigned char pk[samsungsds_aimer128f_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer128f_SECRETKEYBYTES];

    int ret;
    unsigned long long mlen, smlen;

	printf("============= KEYGEN SIGN VERIFY TEST ============\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        samsungsds_aimer128f_aarch64_crypto_sign_keypair(pk, sk);

        samsungsds_aimer128f_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);

        ret = samsungsds_aimer128f_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);

        if(ret)
        {
            printf("Verification failed\n");
            return;
        }
    }

    printf("Verification success\n");    
}

static void TEST_SIGN_AIMER128S(void)
{
    unsigned char m[MLEN + samsungsds_aimer128s_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer128s_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer128s_BYTES];
    unsigned char pk[samsungsds_aimer128s_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer128s_SECRETKEYBYTES];

    int ret;
    unsigned long long mlen, smlen;

	printf("============= KEYGEN SIGN VERIFY TEST ============\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        samsungsds_aimer128s_aarch64_crypto_sign_keypair(pk, sk);

        samsungsds_aimer128s_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);

        ret = samsungsds_aimer128s_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);

        if(ret)
        {
            printf("Verification failed\n");
            return;
        }
    }

    printf("Verification success\n");    
}

static void TEST_SIGN_AIMER192F(void)
{
    unsigned char m[MLEN + samsungsds_aimer192f_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer192f_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer192f_BYTES];
    unsigned char pk[samsungsds_aimer192f_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer192f_SECRETKEYBYTES];

    int ret;
    unsigned long long mlen, smlen;

	printf("============= KEYGEN SIGN VERIFY TEST ============\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        samsungsds_aimer192f_aarch64_crypto_sign_keypair(pk, sk);

        samsungsds_aimer192f_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);

        ret = samsungsds_aimer192f_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);

        if(ret)
        {
            printf("Verification failed\n");
            return;
        }
    }

    printf("Verification success\n");    
}

static void TEST_SIGN_AIMER192S(void)
{
    unsigned char m[MLEN + samsungsds_aimer192s_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer192s_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer192s_BYTES];
    unsigned char pk[samsungsds_aimer192s_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer192s_SECRETKEYBYTES];

    int ret;
    unsigned long long mlen, smlen;

	printf("============= KEYGEN SIGN VERIFY TEST ============\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        samsungsds_aimer192s_aarch64_crypto_sign_keypair(pk, sk);

        samsungsds_aimer192s_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);

        ret = samsungsds_aimer192s_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);

        if(ret)
        {
            printf("Verification failed\n");
            return;
        }
    }

    printf("Verification success\n");    
}

static void TEST_SIGN_AIMER256F(void)
{
    unsigned char m[MLEN + samsungsds_aimer256f_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer256f_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer256f_BYTES];
    unsigned char pk[samsungsds_aimer256f_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer256f_SECRETKEYBYTES];

    int ret;
    unsigned long long mlen, smlen;

	printf("============= KEYGEN SIGN VERIFY TEST ============\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        samsungsds_aimer256f_aarch64_crypto_sign_keypair(pk, sk);

        samsungsds_aimer256f_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);

        ret = samsungsds_aimer256f_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);

        if(ret)
        {
            printf("Verification failed\n");
            return;
        }
    }

    printf("Verification success\n");    
}

static void TEST_SIGN_AIMER256S(void)
{
    unsigned char m[MLEN + samsungsds_aimer256s_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer256s_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer256s_BYTES];
    unsigned char pk[samsungsds_aimer256s_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer256s_SECRETKEYBYTES];

    int ret;
    unsigned long long mlen, smlen;

	printf("============= KEYGEN SIGN VERIFY TEST ============\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        samsungsds_aimer256s_aarch64_crypto_sign_keypair(pk, sk);

        samsungsds_aimer256s_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);

        ret = samsungsds_aimer256s_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);

        if(ret)
        {
            printf("Verification failed\n");
            return;
        }
    }

    printf("Verification success\n");    
}

static void TEST_SIGN_CLOCK_AIMER128F(void)
{
    unsigned char m[MLEN + samsungsds_aimer128f_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer128f_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer128f_BYTES];
    unsigned char pk[samsungsds_aimer128f_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer128f_SECRETKEYBYTES];

    unsigned long long kcycles = 0;
    unsigned long long ecycles = 0;
    unsigned long long dcycles = 0;
	unsigned long long cycles1, cycles2;

    unsigned long long mlen, smlen;

	printf("========== KEYGEN SIGN VERIFY SPEED TEST =========\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer128f_aarch64_crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
		kcycles += cycles2-cycles1 - cyclegap;
    }
    printf("  KEYGEN runs in ................. %8lld cycles\n", kcycles/TEST_LOOP);
	
    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer128f_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);
		cycles2 = cpucycles();
		ecycles += cycles2-cycles1 - cyclegap;

        cycles1 = cpucycles();
        samsungsds_aimer128f_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);
		cycles2 = cpucycles();
		dcycles += cycles2-cycles1 - cyclegap;

    }
    printf("  SIGN   runs in ................. %8lld cycles\n", ecycles/TEST_LOOP);
    printf("  VERIFY runs in ................. %8lld cycles\n", dcycles/TEST_LOOP);
}

static void TEST_SIGN_CLOCK_AIMER128S(void)
{
    unsigned char m[MLEN + samsungsds_aimer128s_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer128s_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer128s_BYTES];
    unsigned char pk[samsungsds_aimer128s_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer128s_SECRETKEYBYTES];

    unsigned long long kcycles = 0;
    unsigned long long ecycles = 0;
    unsigned long long dcycles = 0;
	unsigned long long cycles1, cycles2;

    unsigned long long mlen, smlen;

	printf("========== KEYGEN SIGN VERIFY SPEED TEST =========\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer128s_aarch64_crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
		kcycles += cycles2-cycles1 - cyclegap;
    }
    printf("  KEYGEN runs in ................. %8lld cycles\n", kcycles/TEST_LOOP);
	
    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer128s_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);
		cycles2 = cpucycles();
		ecycles += cycles2-cycles1 - cyclegap;

        cycles1 = cpucycles();
        samsungsds_aimer128s_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);
		cycles2 = cpucycles();
		dcycles += cycles2-cycles1 - cyclegap;

    }
    printf("  SIGN   runs in ................. %8lld cycles\n", ecycles/TEST_LOOP);
    printf("  VERIFY runs in ................. %8lld cycles\n", dcycles/TEST_LOOP);
}

static void TEST_SIGN_CLOCK_AIMER192F(void)
{
    unsigned char m[MLEN + samsungsds_aimer192f_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer192f_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer192f_BYTES];
    unsigned char pk[samsungsds_aimer192f_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer192f_SECRETKEYBYTES];

    unsigned long long kcycles = 0;
    unsigned long long ecycles = 0;
    unsigned long long dcycles = 0;
	unsigned long long cycles1, cycles2;

    unsigned long long mlen, smlen;

	printf("========== KEYGEN SIGN VERIFY SPEED TEST =========\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer192f_aarch64_crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
		kcycles += cycles2-cycles1 - cyclegap;
    }
    printf("  KEYGEN runs in ................. %8lld cycles\n", kcycles/TEST_LOOP);
	
    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer192f_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);
		cycles2 = cpucycles();
		ecycles += cycles2-cycles1 - cyclegap;

        cycles1 = cpucycles();
        samsungsds_aimer192f_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);
		cycles2 = cpucycles();
		dcycles += cycles2-cycles1 - cyclegap;

    }
    printf("  SIGN   runs in ................. %8lld cycles\n", ecycles/TEST_LOOP);
    printf("  VERIFY runs in ................. %8lld cycles\n", dcycles/TEST_LOOP);
}

static void TEST_SIGN_CLOCK_AIMER192S(void)
{
    unsigned char m[MLEN + samsungsds_aimer192s_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer192s_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer192s_BYTES];
    unsigned char pk[samsungsds_aimer192s_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer192s_SECRETKEYBYTES];

    unsigned long long kcycles = 0;
    unsigned long long ecycles = 0;
    unsigned long long dcycles = 0;
	unsigned long long cycles1, cycles2;

    unsigned long long mlen, smlen;

	printf("========== KEYGEN SIGN VERIFY SPEED TEST =========\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer192s_aarch64_crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
		kcycles += cycles2-cycles1 - cyclegap;
    }
    printf("  KEYGEN runs in ................. %8lld cycles\n", kcycles/TEST_LOOP);
	
    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer192s_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);
		cycles2 = cpucycles();
		ecycles += cycles2-cycles1 - cyclegap;

        cycles1 = cpucycles();
        samsungsds_aimer192s_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);
		cycles2 = cpucycles();
		dcycles += cycles2-cycles1 - cyclegap;

    }
    printf("  SIGN   runs in ................. %8lld cycles\n", ecycles/TEST_LOOP);
    printf("  VERIFY runs in ................. %8lld cycles\n", dcycles/TEST_LOOP);
}

static void TEST_SIGN_CLOCK_AIMER256F(void)
{
    unsigned char m[MLEN + samsungsds_aimer256f_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer256f_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer256f_BYTES];
    unsigned char pk[samsungsds_aimer256f_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer256f_SECRETKEYBYTES];

    unsigned long long kcycles = 0;
    unsigned long long ecycles = 0;
    unsigned long long dcycles = 0;
	unsigned long long cycles1, cycles2;

    unsigned long long mlen, smlen;

	printf("========== KEYGEN SIGN VERIFY SPEED TEST =========\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer256f_aarch64_crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
		kcycles += cycles2-cycles1 - cyclegap;
    }
    printf("  KEYGEN runs in ................. %8lld cycles\n", kcycles/TEST_LOOP);
	
    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer256f_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);
		cycles2 = cpucycles();
		ecycles += cycles2-cycles1 - cyclegap;

        cycles1 = cpucycles();
        samsungsds_aimer256f_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);
		cycles2 = cpucycles();
		dcycles += cycles2-cycles1 - cyclegap;

    }
    printf("  SIGN   runs in ................. %8lld cycles\n", ecycles/TEST_LOOP);
    printf("  VERIFY runs in ................. %8lld cycles\n", dcycles/TEST_LOOP);
}

static void TEST_SIGN_CLOCK_AIMER256S(void)
{
    unsigned char m[MLEN + samsungsds_aimer256s_BYTES];
    unsigned char m2[MLEN + samsungsds_aimer256s_BYTES];
    unsigned char sm[MLEN + samsungsds_aimer256s_BYTES];
    unsigned char pk[samsungsds_aimer256s_PUBLICKEYBYTES];
    unsigned char sk[samsungsds_aimer256s_SECRETKEYBYTES];

    unsigned long long kcycles = 0;
    unsigned long long ecycles = 0;
    unsigned long long dcycles = 0;
	unsigned long long cycles1, cycles2;

    unsigned long long mlen, smlen;

	printf("========== KEYGEN SIGN VERIFY SPEED TEST =========\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer256s_aarch64_crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
		kcycles += cycles2-cycles1 - cyclegap;
    }
    printf("  KEYGEN runs in ................. %8lld cycles\n", kcycles/TEST_LOOP);
	
    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        samsungsds_aimer256s_aarch64_crypto_sign(sm, &smlen, m, MLEN, sk);
		cycles2 = cpucycles();
		ecycles += cycles2-cycles1 - cyclegap;

        cycles1 = cpucycles();
        samsungsds_aimer256s_aarch64_crypto_sign_open(m2, &mlen, sm, smlen, pk);
		cycles2 = cpucycles();
		dcycles += cycles2-cycles1 - cyclegap;

    }
    printf("  SIGN   runs in ................. %8lld cycles\n", ecycles/TEST_LOOP);
    printf("  VERIFY runs in ................. %8lld cycles\n", dcycles/TEST_LOOP);
}

int main(void)
{
    setup_rdtsc();

    TEST_SIGN_AIMER128F();
    TEST_SIGN_AIMER128S();
    TEST_SIGN_AIMER192F();
    TEST_SIGN_AIMER192S();
    TEST_SIGN_AIMER256F();
    TEST_SIGN_AIMER256S();

    TEST_SIGN_CLOCK_AIMER128F();
    TEST_SIGN_CLOCK_AIMER128S();
    TEST_SIGN_CLOCK_AIMER192F();
    TEST_SIGN_CLOCK_AIMER192S();
    TEST_SIGN_CLOCK_AIMER256F();
    TEST_SIGN_CLOCK_AIMER256S();

    return 0;
}

