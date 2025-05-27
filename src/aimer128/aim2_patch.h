// SPDX-License-Identifier: MIT

#ifndef AIM2_PATCH_H
#define AIM2_PATCH_H

// Make internal functions static to avoid multiple definition errors
#define GF_exp_invmer_e_1_2 static GF_exp_invmer_e_1_2_internal
#define GF_exp_mer_e_star static GF_exp_mer_e_star_internal
#define generate_matrices_L_and_U static generate_matrices_L_and_U_internal
#define generate_matrix_LU static generate_matrix_LU_internal

#endif // AIM2_PATCH_H
