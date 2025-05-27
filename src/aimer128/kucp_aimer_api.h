#ifndef KUCP_AIMER_API_H
#define KUCP_AIMER_API_H

#define kucp_aimer128f_PUBLICKEYBYTES 32
#define kucp_aimer128s_PUBLICKEYBYTES 32

#define kucp_aimer128f_SECRETKEYBYTES 48
#define kucp_aimer128s_SECRETKEYBYTES 48

#define kucp_aimer128f_BYTES 5888
#define kucp_aimer128s_BYTES 4160

int kucp_aimer128f_aarch64_sign_keypair(unsigned char *pk, unsigned char *sk);
int kucp_aimer128s_aarch64_sign_keypair(unsigned char *pk, unsigned char *sk);

int kucp_aimer128f_aarch64_sign(unsigned char *sm, unsigned long long *smlen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *sk);
int kucp_aimer128s_aarch64_sign(unsigned char *sm, unsigned long long *smlen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *sk);

int kucp_aimer128f_aarch64_sign_open(unsigned char *m, unsigned long long *mlen,
        const unsigned char *sm, unsigned long long smlen,
        const unsigned char *pk);
int kucp_aimer128s_aarch64_sign_open(unsigned char *m, unsigned long long *mlen,
        const unsigned char *sm, unsigned long long smlen,
        const unsigned char *pk);

#endif
