#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "api.h"
#include "rng.h"
#include "sign.h"
#include "cpucycles.h"

#define MLEN 59
#define TEST_LOOP 1000

unsigned long long cyclegap = 0;

static void TEST_SIGN(void)
{
    unsigned char m[MLEN + CRYPTO_BYTES];
    unsigned char m2[MLEN + CRYPTO_BYTES];
    unsigned char sm[MLEN + CRYPTO_BYTES];
    unsigned char pk[CRYPTO_PUBLICKEYBYTES];
    unsigned char sk[CRYPTO_SECRETKEYBYTES];

    int ret;
    unsigned long long mlen, smlen;

	printf("============= KEYGEN SIGN VERIFY TEST ============\n");

    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        crypto_sign_keypair(pk, sk);

        crypto_sign(sm, &smlen, m, MLEN, sk);

        ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);

        if(ret)
        {
            printf("Verification failed\n");
            return;
        }
    }

    printf("Verification success\n");    
}

static void TEST_SIGN_CLOCK(void)
{   
    
    unsigned char m[MLEN + CRYPTO_BYTES];
    unsigned char m2[MLEN + CRYPTO_BYTES];
    unsigned char sm[MLEN + CRYPTO_BYTES];
    unsigned char pk[CRYPTO_PUBLICKEYBYTES];
    unsigned char sk[CRYPTO_SECRETKEYBYTES];

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
        crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
		kcycles += cycles2-cycles1 - cyclegap;
    }
    printf("  KEYGEN runs in ................. %8lld cycles\n", kcycles/TEST_LOOP);
	
    for(int i = 0; i < TEST_LOOP; i++)
    {
        randombytes(m, MLEN);

        cycles1 = cpucycles();
        crypto_sign(sm, &smlen, m, MLEN, sk);
		cycles2 = cpucycles();
		ecycles += cycles2-cycles1 - cyclegap;

        cycles1 = cpucycles();
        crypto_sign_open(m2, &mlen, sm, smlen, pk);
		cycles2 = cpucycles();
		dcycles += cycles2-cycles1 - cyclegap;

    }
    printf("  SIGN   runs in ................. %8lld cycles\n", ecycles/TEST_LOOP);
    printf("  VERIFY runs in ................. %8lld cycles\n", dcycles/TEST_LOOP);
}

int main(void)
{
    printf("CRYPTO_ALGNAME        = %s\n", CRYPTO_ALGNAME);
    printf("CRYPTO_PUBLICKEYBYTES = %d\n", CRYPTO_PUBLICKEYBYTES);
    printf("CRYPTO_SECRETKEYBYTES = %d\n", CRYPTO_SECRETKEYBYTES);
    printf("CRYPTO_BYTES          = %d\n", CRYPTO_BYTES);

    setup_rdtsc();
    TEST_SIGN();
    int i = 0;
    while(i<5){
    TEST_SIGN_CLOCK();
    i++;
    }

    return 0;
}
