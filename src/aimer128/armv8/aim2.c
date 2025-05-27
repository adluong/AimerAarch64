// SPDX-License-Identifier: MIT

#include "aim2.h"
#include "field.h"
#include "hash.h"
#include "params.h"
#include <stddef.h>
#include <stdint.h>

// inverse Mersenne S-box with e1 = 49
// (2 ^ 49 - 1) ^ (-1) mod (2 ^ 128 - 1) = 0xb6b6d6d6dadb5b5b6b6b6d6dadadb5b5
// b6b6d6d 6 d a d b5 b5 b6 b6b6d6d a d a d b5 b5
// Compute in ^ (0xb6) and in ^ (0xb5)
// e_1: table_b_e1 = in1 ^ (0xb6), table_5_e1 = in1 ^ (0xb5)
// e_2: t3_e2 = in2 ^ (0xb6), table_b_e2 = in2 ^ (0xb5)

void GF_exp_invmer_e_1_2(GF out1, GF out2, const GF in1, const GF in2)
{
    // Variables for e_1
    GF t1_e1 = {0}, t2_e1 = {0};
    GF table_5_e1 = {0}, table_6_e1 = {0};
    GF table_a_e1 = {0}, table_b_e1 = {0}, table_d_e1 = {0};
    // Variables for e_2
    GF t1_e2 = {0}, t2_e2 = {0}, t3_e2 = {0};
    GF table_5_e2 = {0}, table_6_e2 = {0};
    GF table_a_e2 = {0}, table_b_e2 = {0}, table_d_e2 = {0};

    // Shared precomputation: Compute small powers for both inputs
    // e_1: t1_e1 = in1 ^ 4
    // e_2: t1_e2 = in2 ^ 4
    GF_sqr(table_d_e1, in1);
    GF_sqr(table_d_e2, in2);
    GF_sqr(t1_e1, table_d_e1);
    GF_sqr(t1_e2, table_d_e2);

    // e_1: table_5_e1 = in1 ^ 5, table_6_e1 = in1 ^ 6
    // e_2: table_5_e2 = in2 ^ 5, table_6_e2 = in2 ^ 6
    GF_mul(table_5_e1, t1_e1, in1);
    GF_mul(table_5_e2, t1_e2, in2);
    GF_mul(table_6_e1, table_5_e1, in1);
    GF_mul(table_6_e2, table_5_e2, in2);
    // e_1: table_a_e1 = in1 ^ 10, table_b_e1 = in1 ^ 11, table_d_e1 = in1 ^ 13
    // e_2: table_a_e2 = in2 ^ 10, table_b_e2 = in2 ^ 11, table_d_e2 = in2 ^ 13
    GF_sqr(table_a_e1, table_5_e1);
    GF_sqr(table_a_e2, table_5_e2);
    GF_mul(table_b_e1, table_a_e1, in1);
    GF_mul(table_b_e2, table_a_e2, in2);
    GF_mul(table_d_e1, table_b_e1, table_d_e1);
    GF_mul(table_d_e2, table_b_e2, table_d_e2);
    GF_sqr(t1_e1, table_b_e1);
    GF_sqr(t1_e2, table_b_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(table_b_e1, t1_e1, table_6_e1);
    GF_mul(table_b_e2, t1_e2, table_5_e2);
    GF_mul(table_5_e1, t1_e1, table_5_e1);
    GF_mul(t3_e2, t1_e2, table_6_e2);
    GF_sqr(t1_e1, table_b_e1);
    GF_sqr(t1_e2, t3_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t2_e2, t1_e2, table_d_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t2_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_b_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_d_e1);
    GF_mul(t1_e2, t1_e2, table_b_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_6_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t2_e1, t1_e1, table_d_e1); //70
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t2_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, t2_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_6_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, table_a_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_d_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, table_d_e2); //238
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_a_e1); //91
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_d_e1);
    GF_sqr(t1_e1, t1_e1);  //1
    GF_sqr(t1_e2, t1_e2);  //1
    GF_sqr(t1_e1, t1_e1);  //2
    GF_sqr(t1_e2, t1_e2);  //2
    GF_sqr(t1_e1, t1_e1);  //3
    GF_sqr(t1_e2, t1_e2);  //3
    GF_sqr(t1_e1, t1_e1);  //4
    GF_sqr(t1_e2, t1_e2);  //4
    GF_sqr(t1_e1, t1_e1);  //5
    GF_sqr(t1_e2, t1_e2);  //5  
    GF_sqr(t1_e1, t1_e1);  //6
    GF_mul(t1_e2, t1_e2, t2_e2);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);  //7
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);  //8
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_5_e1);   //line 105
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, table_a_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);  //11
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);  //12
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, table_d_e2); 
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1); 
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_5_e1);  //line 112
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);  //6
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_b_e1); //line 119
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, t2_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, table_6_e2);  //line 273
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, table_d_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);  //7
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);  //8
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);  //9
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);  //10
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, t2_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, t2_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, table_6_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_a_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, table_d_e2); //line 301
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_d_e1); //line 140
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_a_e1);  //line 147
    GF_mul(t1_e2, t1_e2, t3_e2);       //line 308
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_mul(t1_e1, t1_e1, table_d_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(t1_e2, t1_e2, t2_e2);
    GF_mul(t1_e1, t1_e1, table_5_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_sqr(t1_e2, t1_e2);
    GF_sqr(t1_e1, t1_e1);
    GF_mul(out2, t1_e2, table_b_e2);
    GF_mul(out1, t1_e1, table_5_e1);
}

// Mersenne exponentiation with e_star = 3
void GF_exp_mer_e_star(GF out, const GF in)
{
  GF t1 = {0,};

  // t1 = a ^ (2 ^ 2 - 1)
  GF_sqr(t1, in);
  GF_mul(t1, t1, in);

  // out = a ^ (2 ^ 3 - 1)
  GF_sqr(t1, t1);
  GF_mul(out, t1, in);
}

void generate_matrices_L_and_U(
        GF matrix_L[AIM2_NUM_INPUT_SBOX][AIM2_NUM_BITS_FIELD],
        GF matrix_U[AIM2_NUM_INPUT_SBOX][AIM2_NUM_BITS_FIELD],
        GF vector_b,
        const uint8_t iv[AIM2_IV_SIZE])
{
  uint8_t buf[AIM2_NUM_BYTES_FIELD];
  uint64_t ormask, lmask, umask;
  hash_instance ctx;
  GF temp = {0,};

  // initialize hash
  hash_init(&ctx);
  hash_update(&ctx, iv, AIM2_IV_SIZE);
  hash_final(&ctx);

  for (size_t num = 0; num < AIM2_NUM_INPUT_SBOX; num++)
  {
    for (size_t row = 0; row < AIM2_NUM_BITS_FIELD; row++)
    {
      hash_squeeze(&ctx, buf, AIM2_NUM_BYTES_FIELD);
      GF_from_bytes(temp, buf);

      ormask = ((uint64_t)1) << (row % 64);
      lmask = ((uint64_t)-1) << (row % 64);
      umask = ~lmask;

      size_t inter = row / 64;
      size_t col_word;
      for (col_word = 0; col_word < inter; col_word++)
      {
        // L is zero, U is full
        matrix_L[num][row][col_word] = 0;
        matrix_U[num][row][col_word] = temp[col_word];
      }
      matrix_L[num][row][inter] = (temp[inter] & lmask) | ormask;
      matrix_U[num][row][inter] = (temp[inter] & umask) | ormask;
      for (col_word = inter + 1; col_word < AIM2_NUM_WORDS_FIELD; col_word++)
      {
        // L is full, U is zero
        matrix_L[num][row][col_word] = temp[col_word];
        matrix_U[num][row][col_word] = 0;
      }
    }
  }

  hash_squeeze(&ctx, (uint8_t *)vector_b, AIM2_NUM_BYTES_FIELD);
  hash_ctx_release(&ctx);
}

void generate_matrix_LU(GF matrix_A[AIM2_NUM_INPUT_SBOX][AIM2_NUM_BITS_FIELD],
                        GF vector_b,
                        const uint8_t iv[AIM2_IV_SIZE])
{
  GF matrix_L[AIM2_NUM_INPUT_SBOX][AIM2_NUM_BITS_FIELD];
  GF matrix_U[AIM2_NUM_INPUT_SBOX][AIM2_NUM_BITS_FIELD];

  generate_matrices_L_and_U(matrix_L, matrix_U, vector_b, iv);

  for (size_t num = 0; num < AIM2_NUM_INPUT_SBOX; num++)
  {
    for (size_t i = 0; i < AIM2_NUM_BITS_FIELD; i++)
    {
      GF_transposed_matmul(matrix_A[num][i], matrix_U[num][i],(const GF *)matrix_L[num]);
    }
  }
}

void aim2(uint8_t ct[AIM2_NUM_BYTES_FIELD],
          const uint8_t pt[AIM2_NUM_BYTES_FIELD],
          const uint8_t iv[AIM2_IV_SIZE])
{
  GF matrix_L[AIM2_NUM_INPUT_SBOX][AIM2_NUM_BITS_FIELD];
  GF matrix_U[AIM2_NUM_INPUT_SBOX][AIM2_NUM_BITS_FIELD];
  GF vector_b = {0,};

  GF state[AIM2_NUM_INPUT_SBOX];
  GF pt_GF = {0,}, ct_GF = {0,};
  GF_from_bytes(pt_GF, pt);

  // generate random matrix
  generate_matrices_L_and_U(matrix_L, matrix_U, vector_b, iv);

  // linear component: constant addition
  GF_add(state[0], pt_GF, aim2_constants[0]);
  GF_add(state[1], pt_GF, aim2_constants[1]);
  
  // non-linear component: inverse Mersenne S-box
  GF_exp_invmer_e_1_2(state[0],state[1],state[0],state[1]);

  // linear component: affine layer
  GF_transposed_matmul(state[0], state[0], (const GF *)matrix_U[0]);
  GF_transposed_matmul(state[1], state[1], (const GF *)matrix_U[1]);
  GF_transposed_matmul(state[0], state[0], (const GF *)matrix_L[0]);
  GF_transposed_matmul(state[1], state[1], (const GF *)matrix_L[1]);

  GF_add(state[0], state[0], state[1]);
  GF_add(state[0], state[0], vector_b);

  // non-linear component: Mersenne S-box
  GF_exp_mer_e_star(state[0], state[0]);

  // linear component: feed-forward
  GF_add(ct_GF, state[0], pt_GF);

  GF_to_bytes(ct, ct_GF);
}

void aim2_sbox_outputs(GF sbox_outputs[AIM2_NUM_INPUT_SBOX], const GF pt)
{
  // linear component: constant addition
  GF_add(sbox_outputs[0], pt, aim2_constants[0]);
  GF_add(sbox_outputs[1], pt, aim2_constants[1]);

  // non-linear component: inverse Mersenne S-box
  GF_exp_invmer_e_1_2(sbox_outputs[0],sbox_outputs[1],sbox_outputs[0],sbox_outputs[1]);
}
