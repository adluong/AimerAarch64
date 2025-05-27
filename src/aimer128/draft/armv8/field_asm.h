// SPDX-License-Identifier: MIT

#ifndef FIELD_ASM_H
#define FIELD_ASM_H

#include "params.h"
#include "field.h"

// Assembly function declarations based on variant
#ifdef AIMER128S
// AIMER-128s assembly functions
extern void samsungsds_aimer128s_aarch64_GF_set0(GF a);
extern void samsungsds_aimer128s_aarch64_GF_copy(GF out, const GF in);
extern void samsungsds_aimer128s_aarch64_GF_add(GF c, const GF a, const GF b);
extern void samsungsds_aimer128s_aarch64_GF_mul(GF c, const GF a, const GF b);
extern void samsungsds_aimer128s_aarch64_GF_mul_N(GF c[AIMER_N], const GF a[AIMER_N], const GF b);
extern void samsungsds_aimer128s_aarch64_GF_mul_add(GF c, const GF a, const GF b);
extern void samsungsds_aimer128s_aarch64_GF_mul_add_N(GF c[AIMER_N], const GF a[AIMER_N], const GF b);
extern void samsungsds_aimer128s_aarch64_GF_sqr(GF c, const GF a);
extern void samsungsds_aimer128s_aarch64_GF_sqr_N(GF c[AIMER_N], const GF a[AIMER_N]);
extern void samsungsds_aimer128s_aarch64_POLY_mul_add_N(GF lo[AIMER_N], GF hi[AIMER_N], const GF a[AIMER_N], const GF b);
extern void samsungsds_aimer128s_aarch64_POLY_red_N(GF lo[AIMER_N], const GF hi[AIMER_N]);

// Map generic names to 128s functions
#define GF_set0(a) samsungsds_aimer128s_aarch64_GF_set0(a)
#define GF_copy(out, in) samsungsds_aimer128s_aarch64_GF_copy(out, in)
#define GF_add(c, a, b) samsungsds_aimer128s_aarch64_GF_add(c, a, b)
#define GF_mul(c, a, b) samsungsds_aimer128s_aarch64_GF_mul(c, a, b)
#define GF_mul_N(c, a, b) samsungsds_aimer128s_aarch64_GF_mul_N(c, a, b)
#define GF_mul_add(c, a, b) samsungsds_aimer128s_aarch64_GF_mul_add(c, a, b)
#define GF_mul_add_N(c, a, b) samsungsds_aimer128s_aarch64_GF_mul_add_N(c, a, b)
#define GF_sqr(c, a) samsungsds_aimer128s_aarch64_GF_sqr(c, a)
#define GF_sqr_N(c, a) samsungsds_aimer128s_aarch64_GF_sqr_N(c, a)
#define POLY_mul_add_N(lo, hi, a, b) samsungsds_aimer128s_aarch64_POLY_mul_add_N(lo, hi, a, b)
#define POLY_red_N(lo, hi) samsungsds_aimer128s_aarch64_POLY_red_N(lo, hi)

#else
// AIMER-128f assembly functions (default)
extern void samsungsds_aimer128f_aarch64_GF_set0(GF a);
extern void samsungsds_aimer128f_aarch64_GF_copy(GF out, const GF in);
extern void samsungsds_aimer128f_aarch64_GF_add(GF c, const GF a, const GF b);
extern void samsungsds_aimer128f_aarch64_GF_mul(GF c, const GF a, const GF b);
extern void samsungsds_aimer128f_aarch64_GF_mul_N(GF c[AIMER_N], const GF a[AIMER_N], const GF b);
extern void samsungsds_aimer128f_aarch64_GF_mul_add(GF c, const GF a, const GF b);
extern void samsungsds_aimer128f_aarch64_GF_mul_add_N(GF c[AIMER_N], const GF a[AIMER_N], const GF b);
extern void samsungsds_aimer128f_aarch64_GF_sqr(GF c, const GF a);
extern void samsungsds_aimer128f_aarch64_GF_sqr_N(GF c[AIMER_N], const GF a[AIMER_N]);
extern void samsungsds_aimer128f_aarch64_POLY_mul_add_N(GF lo[AIMER_N], GF hi[AIMER_N], const GF a[AIMER_N], const GF b);
extern void samsungsds_aimer128f_aarch64_POLY_red_N(GF lo[AIMER_N], const GF hi[AIMER_N]);

// Map generic names to 128f functions
#define GF_set0(a) samsungsds_aimer128f_aarch64_GF_set0(a)
#define GF_copy(out, in) samsungsds_aimer128f_aarch64_GF_copy(out, in)
#define GF_add(c, a, b) samsungsds_aimer128f_aarch64_GF_add(c, a, b)
#define GF_mul(c, a, b) samsungsds_aimer128f_aarch64_GF_mul(c, a, b)
#define GF_mul_N(c, a, b) samsungsds_aimer128f_aarch64_GF_mul_N(c, a, b)
#define GF_mul_add(c, a, b) samsungsds_aimer128f_aarch64_GF_mul_add(c, a, b)
#define GF_mul_add_N(c, a, b) samsungsds_aimer128f_aarch64_GF_mul_add_N(c, a, b)
#define GF_sqr(c, a) samsungsds_aimer128f_aarch64_GF_sqr(c, a)
#define GF_sqr_N(c, a) samsungsds_aimer128f_aarch64_GF_sqr_N(c, a)
#define POLY_mul_add_N(lo, hi, a, b) samsungsds_aimer128f_aarch64_POLY_mul_add_N(lo, hi, a, b)
#define POLY_red_N(lo, hi) samsungsds_aimer128f_aarch64_POLY_red_N(lo, hi)

#endif

#endif // FIELD_ASM_H
