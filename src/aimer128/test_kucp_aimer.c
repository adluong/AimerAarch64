// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "kucp_aimer_api.h"

// Color codes for output
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m"

void print_hex(const char* label, const unsigned char* data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len && i < 32; i++) {
        printf("%02x", data[i]);
    }
    if (len > 32) printf("...");
    printf("\n");
}

int test_aimer_variant(const char* variant_name, 
                       int pk_size, int sk_size, int sig_size,
                       int (*keypair_func)(unsigned char*, unsigned char*),
                       int (*sign_func)(unsigned char*, unsigned long long*, 
                                       const unsigned char*, unsigned long long, 
                                       const unsigned char*),
                       int (*open_func)(unsigned char*, unsigned long long*,
                                       const unsigned char*, unsigned long long,
                                       const unsigned char*)) {
    
    printf("\n%s========== Testing %s ==========%s\n", BLUE, variant_name, RESET);
    printf("Public key size: %d bytes\n", pk_size);
    printf("Secret key size: %d bytes\n", sk_size);
    printf("Signature size: %d bytes\n", sig_size);
    
    unsigned char pk[pk_size];
    unsigned char sk[sk_size];
    
    // Test key generation
    printf("\n%sTesting key generation...%s\n", YELLOW, RESET);
    int ret = keypair_func(pk, sk);
    if (ret != 0) {
        printf("%s[FAIL]%s Key generation failed\n", RED, RESET);
        return 1;
    }
    printf("%s[PASS]%s Key generation successful\n", GREEN, RESET);
    print_hex("Public key", pk, pk_size);
    print_hex("Secret key", sk, sk_size);
    
    // Test signing and verification
    const char* test_msg = "Hello, AIMER signature scheme!";
    size_t msg_len = strlen(test_msg);
    unsigned char* msg = (unsigned char*)test_msg;
    unsigned char sm[msg_len + sig_size];
    unsigned char msg2[msg_len];
    unsigned long long smlen, mlen2;
    
    printf("\n%sTesting signing...%s\n", YELLOW, RESET);
    printf("Message: %s\n", test_msg);
    
    ret = sign_func(sm, &smlen, msg, msg_len, sk);
    if (ret != 0 || smlen != msg_len + sig_size) {
        printf("%s[FAIL]%s Signing failed (ret=%d, smlen=%llu, expected=%zu)\n", 
               RED, RESET, ret, smlen, msg_len + sig_size);
        return 1;
    }
    printf("%s[PASS]%s Message signed successfully\n", GREEN, RESET);
    printf("Signed message length: %llu bytes\n", smlen);
    
    printf("\n%sTesting verification...%s\n", YELLOW, RESET);
    ret = open_func(msg2, &mlen2, sm, smlen, pk);
    if (ret != 0 || mlen2 != msg_len || memcmp(msg, msg2, msg_len) != 0) {
        printf("%s[FAIL]%s Verification failed\n", RED, RESET);
        return 1;
    }
    printf("%s[PASS]%s Message verified successfully\n", GREEN, RESET);
    
    // Test tamper detection
    printf("\n%sTesting tamper detection...%s\n", YELLOW, RESET);
    sm[msg_len/2] ^= 0x01;  // Tamper with message
    ret = open_func(msg2, &mlen2, sm, smlen, pk);
    if (ret == 0) {
        printf("%s[FAIL]%s Tampering not detected\n", RED, RESET);
        return 1;
    }
    printf("%s[PASS]%s Message tampering detected\n", GREEN, RESET);
    
    return 0;
}

int main() {
    printf("%s╔══════════════════════════════════════════╗%s\n", BLUE, RESET);
    printf("%s║      KUCP AIMER Library Test Suite        ║%s\n", BLUE, RESET);
    printf("%s╚══════════════════════════════════════════╝%s\n", BLUE, RESET);
    
    int all_passed = 1;
    
    // Test AIMER-128s
    if (test_aimer_variant("AIMER-128s", 
                          kucp_aimer128s_PUBLICKEYBYTES,
                          kucp_aimer128s_SECRETKEYBYTES,
                          kucp_aimer128s_BYTES,
                          kucp_aimer128s_aarch64_sign_keypair,
                          kucp_aimer128s_aarch64_sign,
                          kucp_aimer128s_aarch64_sign_open) != 0) {
        all_passed = 0;
    }
    
    // Test AIMER-128f
    if (test_aimer_variant("AIMER-128f", 
                          kucp_aimer128f_PUBLICKEYBYTES,
                          kucp_aimer128f_SECRETKEYBYTES,
                          kucp_aimer128f_BYTES,
                          kucp_aimer128f_aarch64_sign_keypair,
                          kucp_aimer128f_aarch64_sign,
                          kucp_aimer128f_aarch64_sign_open) != 0) {
        all_passed = 0;
    }
    
    // Summary
    printf("\n%s========== Test Summary ==========%s\n", BLUE, RESET);
    if (all_passed) {
        printf("%s[ALL TESTS PASSED]%s\n", GREEN, RESET);
        printf("\nThe KUCP AIMER library is working correctly!\n");
        return 0;
    } else {
        printf("%s[SOME TESTS FAILED]%s\n", RED, RESET);
        return 1;
    }
}
