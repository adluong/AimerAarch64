// SPDX-License-Identifier: MIT

#include "kucp_aimer_api.h"
#include <stddef.h>
#include <stdint.h>

// External function declarations for AIMER-128s
extern int samsungsds_aimer128s_aarch64_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int samsungsds_aimer128s_aarch64_crypto_sign(uint8_t *sm, size_t *smlen,
        const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int samsungsds_aimer128s_aarch64_crypto_sign_open(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen, const uint8_t *pk);

// External function declarations for AIMER-128f
extern int samsungsds_aimer128f_aarch64_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int samsungsds_aimer128f_aarch64_crypto_sign(uint8_t *sm, size_t *smlen,
        const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int samsungsds_aimer128f_aarch64_crypto_sign_open(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen, const uint8_t *pk);

// AIMER-128f wrapper functions
int kucp_aimer128f_aarch64_sign_keypair(unsigned char *pk, unsigned char *sk)
{
    return samsungsds_aimer128f_aarch64_crypto_sign_keypair(pk, sk);
}

int kucp_aimer128f_aarch64_sign(unsigned char *sm, unsigned long long *smlen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *sk)
{
    size_t len;
    int ret = samsungsds_aimer128f_aarch64_crypto_sign(sm, &len, m, (size_t)mlen, sk);
    *smlen = (unsigned long long)len;
    return ret;
}

int kucp_aimer128f_aarch64_sign_open(unsigned char *m, unsigned long long *mlen,
        const unsigned char *sm, unsigned long long smlen,
        const unsigned char *pk)
{
    size_t len;
    int ret = samsungsds_aimer128f_aarch64_crypto_sign_open(m, &len, sm, (size_t)smlen, pk);
    *mlen = (unsigned long long)len;
    return ret;
}

// AIMER-128s wrapper functions
int kucp_aimer128s_aarch64_sign_keypair(unsigned char *pk, unsigned char *sk)
{
    return samsungsds_aimer128s_aarch64_crypto_sign_keypair(pk, sk);
}

int kucp_aimer128s_aarch64_sign(unsigned char *sm, unsigned long long *smlen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *sk)
{
    size_t len;
    int ret = samsungsds_aimer128s_aarch64_crypto_sign(sm, &len, m, (size_t)mlen, sk);
    *smlen = (unsigned long long)len;
    return ret;
}

int kucp_aimer128s_aarch64_sign_open(unsigned char *m, unsigned long long *mlen,
        const unsigned char *sm, unsigned long long smlen,
        const unsigned char *pk)
{
    size_t len;
    int ret = samsungsds_aimer128s_aarch64_crypto_sign_open(m, &len, sm, (size_t)smlen, pk);
    *mlen = (unsigned long long)len;
    return ret;
}
