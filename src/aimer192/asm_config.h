// SPDX-License-Identifier: MIT

#ifndef ASM_CONFIG_H
#define ASM_CONFIG_H

// Define loop counts based on AIMER_N
#ifdef AIMER128S
  // AIMER_N = 256, so 256/8 = 32
  #define AIMER_N_DIV_8 32
#else
  // AIMER128F: AIMER_N = 16, so 16/8 = 2
  #define AIMER_N_DIV_8 2
#endif

#endif // ASM_CONFIG_H
