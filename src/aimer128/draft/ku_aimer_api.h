#ifndef KUCP_AIMER_API_H
#define KUCP_AIMER_API_H

#define samsungsds_aimer128f_PUBLICKEYBYTES 32
#define samsungsds_aimer128s_PUBLICKEYBYTES 32



#define samsungsds_aimer128f_SECRETKEYBYTES 48
#define samsungsds_aimer128s_SECRETKEYBYTES 48





#define samsungsds_aimer128f_BYTES 5888
#define samsungsds_aimer128s_BYTES 4160





int samsungsds_aimer128f_aarch64_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int samsungsds_aimer128s_aarch64_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);





int samsungsds_aimer128f_aarch64_crypto_sign(unsigned char *sm, unsigned long long *smlen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *sk);
int samsungsds_aimer128s_aarch64_crypto_sign(unsigned char *sm, unsigned long long *smlen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *sk);













int samsungsds_aimer128f_aarch64_crypto_sign_open(unsigned char *m, unsigned long long *mlen,
        const unsigned char *sm, unsigned long long smlen,
        const unsigned char *pk);
int samsungsds_aimer128s_aarch64_crypto_sign_open(unsigned char *m, unsigned long long *mlen,
        const unsigned char *sm, unsigned long long smlen,
        const unsigned char *pk);













#endif

