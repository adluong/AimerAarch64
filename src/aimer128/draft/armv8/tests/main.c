// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "../api.h"
#include "../field.h"
#include "../aim2.h"
#include "../../../common/rng.h"
#include "../../../common/cpucycles.h"
#include "../../../common/speed_print.h"

#define MLEN 59
#define NTESTS_SIGN 5
#define NTESTS_SPEED 10000

// Function declarations
int test_sign_verify(void);
int test_speed_benchmark(void);
int test_aim2_cipher(void);

// Test 1: Digital signature verification test
int test_sign_verify(void)
{
  printf("=== Running Digital Signature Test ===\n");
  
  size_t i, j;
  size_t mlen, smlen;
  uint8_t b;
  uint8_t m[MLEN + CRYPTO_BYTES];
  uint8_t m2[MLEN + CRYPTO_BYTES];
  uint8_t sm[MLEN + CRYPTO_BYTES];
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];

  for (i = 0; i < NTESTS_SIGN; ++i)
  {
    int ret = 0;

    randombytes(m, MLEN);

    crypto_sign_keypair(pk, sk);
    crypto_sign(sm, &smlen, m, MLEN, sk);

    ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);

    if (ret)
    {
      printf("Verification failed\n");
      return -1;
    }
    if (smlen != MLEN + CRYPTO_BYTES)
    {
      printf("Signed message lengths wrong\n");
      return -1;
    }
    if (mlen != MLEN)
    {
      printf("Message lengths wrong\n");
      return -1;
    }
    for (j = 0; j < MLEN; ++j)
    {
      if (m2[j] != m[j])
      {
        printf("Messages don't match\n");
        return -1;
      }
    }

    randombytes((uint8_t *)&j, sizeof(j));
    do
    {
      randombytes(&b, 1);
    }
    while (!b);
    sm[j % (MLEN + CRYPTO_BYTES)] += b;
    ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);
    if (!ret)
    {
      printf("Trivial forgeries possible\n");
      return -1;
    }
  }

  printf("CRYPTO_PUBLICKEYBYTES = %d\n", CRYPTO_PUBLICKEYBYTES);
  printf("CRYPTO_SECRETKEYBYTES = %d\n", CRYPTO_SECRETKEYBYTES);
  printf("CRYPTO_BYTES = %d\n", CRYPTO_BYTES);
  printf("Digital signature test passed!\n\n");

  return 0;
}

// Test 2: Speed benchmark test
int test_speed_benchmark(void)
{
  printf("=== Running Speed Benchmark Test ===\n");
  
  uint64_t t[NTESTS_SPEED];
  int i;
  int ret = 0;

  size_t mlen = 59;
  size_t smlen = 0;
  uint8_t message[mlen];
  uint8_t m[mlen];
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t sm[mlen + CRYPTO_BYTES];

  uint64_t time_begin, time_end;

  randombytes((unsigned char*)message, 59);

  for(i = 0; i < NTESTS_SPEED; i++)
  {
    time_begin = cpucycles();
    ret = crypto_sign_keypair(pk, sk);
    time_end   = cpucycles();
    t[i] = time_end - time_begin;
  }
  print_results("aimer_keypair: ", t, NTESTS_SPEED);

  for(i = 0; i < NTESTS_SPEED; i++)
  {
    time_begin = cpucycles();
    ret = crypto_sign(sm, &smlen, message, mlen, sk);
    time_end   = cpucycles();
    t[i] = time_end - time_begin;
  }
  print_results("aimer_sign   : ", t, NTESTS_SPEED);

  for(i = 0; i < NTESTS_SPEED; i++)
  {
    time_begin = cpucycles();
    ret = crypto_sign_open(m, &mlen, sm, smlen, pk);
    time_end   = cpucycles();
    t[i] = time_end - time_begin;
  }
  print_results("aimer_verify : ", t, NTESTS_SPEED);

  printf("Speed benchmark test completed!\n\n");

  return ret;
}

// Test 3: AIM2 cipher test
int test_aim2_cipher(void)
{
  printf("=== Running AIM2 Cipher Test ===\n");
  
  int succ = 1, i;

  // Inverted order compared to Sage!
  uint8_t pt[24] =
    {0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
     0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
     0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};
  const uint8_t ct_expected[24] =
    {0x0a, 0x68, 0x3f, 0x52, 0x45, 0xc5, 0x68, 0xb4,
     0xc6, 0x4a, 0xfc, 0x73, 0x4a, 0x51, 0x94, 0xc0,
     0x77, 0xc8, 0x41, 0x35, 0xdf, 0xa5, 0xce, 0xad};
  uint8_t iv[24] =
    {0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
     0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
     0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};

  uint8_t ct[24] = {0,};

  aim2(ct, pt, iv);

  printf("PLAINTEXT                 : ");
  for (i = (int)sizeof(pt) - 1; i >= 0; i--)
  {
    printf("%02x", pt[i]);
  }
  printf("\n");

  printf("IV                        : ");
  for (i = (int)sizeof(iv) - 1; i >= 0; i--)
  {
    printf("%02x", iv[i]);
  }
  printf("\n");

  printf("CIPHERTEXT                : ");
  for (i = (int)sizeof(ct) - 1; i >= 0; i--)
  {
    printf("%02x", ct[i]);
  }
  printf("\n");

  if (memcmp(ct_expected, ct, sizeof(GF)))
  {
    succ = 0;
    printf("[-] ct != ct_expected\n");
  }

  if (succ)
    printf("AIM2 cipher test passed!\n");
  else
    printf("AIM2 cipher test failed!\n");

  printf("\n");

  // For  extracting test data -------------------------------------------------
  // printf("CIPHERTEXT(little endian) : ");
  // for (i = 0; i < (int)sizeof(ct) - 1; i++)
  // {
  //   printf("0x%02x, ", ct[i]);
  // }
  // printf("0x%02x\n", ct[i]);
  // For  extracting test data -------------------------------------------------

  return succ ? 0 : -1;
}

// Main function that runs all tests
int main(void)
{
  printf("=== Combined Cryptographic Test Suite ===\n\n");
  
  int result = 0;
  
  // Run digital signature test
  if (test_sign_verify() != 0)
  {
    printf("Digital signature test failed!\n");
    result = -1;
  }
  
  // Run speed benchmark test
  if (test_speed_benchmark() != 0)
  {
    printf("Speed benchmark test failed!\n");
    result = -1;
  }
  
  // Run AIM2 cipher test
  if (test_aim2_cipher() != 0)
  {
    printf("AIM2 cipher test failed!\n");
    result = -1;
  }
  
  if (result == 0)
  {
    printf("=== All tests passed successfully! ===\n");
  }
  else
  {
    printf("=== Some tests failed! ===\n");
  }
  
  return result;
}
