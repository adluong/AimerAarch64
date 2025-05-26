// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Include the appropriate header based on variant
#ifdef AIMER128S
#include "aimer128s/api.h"
#include "aimer128s/params.h"
#elif AIMER128F
#include "aimer128f/api.h"
#include "aimer128f/params.h"
#else
#include "aimer128f/api.h"
#include "aimer128f/params.h"
#endif

// Test configuration
#define TEST_MESSAGES 5
#define MESSAGE_SIZES {32, 59, 128, 256, 1024}
#define TIMING_ITERATIONS 10

// Color codes for output
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m"

// Function to get current time in microseconds
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000.0 + ts.tv_nsec / 1000.0;
}

// Print hex data
void print_hex(const char* label, const uint8_t* data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len && i < 32; i++) {
        printf("%02x", data[i]);
    }
    if (len > 32) printf("...");
    printf("\n");
}

// Test a single message
int test_message(const uint8_t* pk, const uint8_t* sk, size_t msg_size) {
    uint8_t* msg = malloc(msg_size);
    uint8_t* sm = malloc(msg_size + CRYPTO_BYTES);
    uint8_t* msg2 = malloc(msg_size);
    size_t smlen, mlen2;
    int ret;
    int success = 1;
    
    // Generate random message
    for (size_t i = 0; i < msg_size; i++) {
        msg[i] = rand() & 0xFF;
    }
    
    printf("\nTesting message size: %zu bytes\n", msg_size);
    print_hex("Message", msg, msg_size);
    
    // Sign message
    ret = crypto_sign(sm, &smlen, msg, msg_size, sk);
    if (ret != 0 || smlen != msg_size + CRYPTO_BYTES) {
        printf("%s[FAIL]%s Signing failed\n", RED, RESET);
        success = 0;
        goto cleanup;
    }
    printf("%s[PASS]%s Message signed successfully\n", GREEN, RESET);
    
    // Verify message
    ret = crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
    if (ret != 0 || mlen2 != msg_size || memcmp(msg, msg2, msg_size) != 0) {
        printf("%s[FAIL]%s Verification failed\n", RED, RESET);
        success = 0;
        goto cleanup;
    }
    printf("%s[PASS]%s Message verified successfully\n", GREEN, RESET);
    
    // Test tamper detection - modify message
    sm[msg_size/2] ^= 0x01;
    ret = crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
    if (ret == 0) {
        printf("%s[FAIL]%s Message tampering not detected\n", RED, RESET);
        success = 0;
    } else {
        printf("%s[PASS]%s Message tampering detected\n", GREEN, RESET);
    }
    sm[msg_size/2] ^= 0x01; // Restore
    
    // Test tamper detection - modify signature
    sm[msg_size + CRYPTO_BYTES/2] ^= 0x01;
    ret = crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
    if (ret == 0) {
        printf("%s[FAIL]%s Signature tampering not detected\n", RED, RESET);
        success = 0;
    } else {
        printf("%s[PASS]%s Signature tampering detected\n", GREEN, RESET);
    }
    
cleanup:
    free(msg);
    free(sm);
    free(msg2);
    return success;
}

int main() {
    printf("%s╔══════════════════════════════════════════╗%s\n", BLUE, RESET);
    printf("%s║     AIMER Digital Signature Test Suite    ║%s\n", BLUE, RESET);
    printf("%s╚══════════════════════════════════════════╝%s\n", BLUE, RESET);
    
    // Initialize random seed
    srand(time(NULL));
    
    // Print variant info
#ifdef AIMER128S
    printf("\nTesting AIMER-128s variant\n");
#elif AIMER128F
    printf("\nTesting AIMER-128f variant\n");
#else
    printf("\nTesting AIMER-%s variant\n", CRYPTO_ALGNAME);
#endif
    
    printf("Algorithm: %s\n", CRYPTO_ALGNAME);
    printf("Public key size: %d bytes\n", CRYPTO_PUBLICKEYBYTES);
    printf("Secret key size: %d bytes\n", CRYPTO_SECRETKEYBYTES);
    printf("Signature size: %d bytes\n", CRYPTO_BYTES);
    
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    
    // Test key generation
    printf("\n%sTesting key generation...%s\n", YELLOW, RESET);
    double total_keypair_time = 0;
    
    for (int i = 0; i < TIMING_ITERATIONS; i++) {
        double start = get_time();
        int ret = crypto_sign_keypair(pk, sk);
        double end = get_time();
        total_keypair_time += (end - start);
        
        if (ret != 0) {
            printf("%s[FAIL]%s Key generation failed\n", RED, RESET);
            return 1;
        }
    }
    
    printf("%s[PASS]%s Key generation successful\n", GREEN, RESET);
    printf("Average time: %.2f μs\n", total_keypair_time / TIMING_ITERATIONS);
    
    // Print sample keys
    print_hex("Public key", pk, CRYPTO_PUBLICKEYBYTES);
    print_hex("Secret key", sk, CRYPTO_SECRETKEYBYTES);
    
    // Test signing and verification with different message sizes
    int msg_sizes[] = MESSAGE_SIZES;
    printf("\n%sTesting signing and verification...%s\n", YELLOW, RESET);
    
    int all_passed = 1;
    double total_sign_time = 0;
    double total_verify_time = 0;
    
    for (int i = 0; i < TEST_MESSAGES; i++) {
        size_t msg_size = msg_sizes[i];
        uint8_t* msg = malloc(msg_size);
        uint8_t* sm = malloc(msg_size + CRYPTO_BYTES);
        uint8_t* msg2 = malloc(msg_size);
        size_t smlen, mlen2;
        
        // Generate random message
        for (size_t j = 0; j < msg_size; j++) {
            msg[j] = rand() & 0xFF;
        }
        
        // Time signing
        double start = get_time();
        crypto_sign(sm, &smlen, msg, msg_size, sk);
        double end = get_time();
        total_sign_time += (end - start);
        
        // Time verification
        start = get_time();
        crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
        end = get_time();
        total_verify_time += (end - start);
        
        // Run tests
        if (!test_message(pk, sk, msg_size)) {
            all_passed = 0;
        }
        
        free(msg);
        free(sm);
        free(msg2);
    }
    
    // Performance summary
    printf("\n%sPerformance Summary:%s\n", YELLOW, RESET);
    printf("Average signing time: %.2f μs\n", total_sign_time / TEST_MESSAGES);
    printf("Average verification time: %.2f μs\n", total_verify_time / TEST_MESSAGES);
    
    // Test with signature API
    printf("\n%sTesting signature-only API...%s\n", YELLOW, RESET);
    size_t test_msg_len = 59;
    uint8_t test_msg[59];
    uint8_t sig[CRYPTO_BYTES];
    size_t siglen;
    
    for (size_t i = 0; i < test_msg_len; i++) {
        test_msg[i] = i & 0xFF;
    }
    
    int ret = crypto_sign_signature(sig, &siglen, test_msg, test_msg_len, sk);
    if (ret != 0 || siglen != CRYPTO_BYTES) {
        printf("%s[FAIL]%s Signature generation failed\n", RED, RESET);
        all_passed = 0;
    } else {
        printf("%s[PASS]%s Signature generated successfully\n", GREEN, RESET);
        
        ret = crypto_sign_verify(sig, siglen, test_msg, test_msg_len, pk);
        if (ret != 0) {
            printf("%s[FAIL]%s Signature verification failed\n", RED, RESET);
            all_passed = 0;
        } else {
            printf("%s[PASS]%s Signature verified successfully\n", GREEN, RESET);
        }
    }
    
    // Final summary
    printf("\n%s========== Test Summary ==========%s\n", BLUE, RESET);
    if (all_passed) {
        printf("%s[ALL TESTS PASSED]%s\n", GREEN, RESET);
        return 0;
    } else {
        printf("%s[SOME TESTS FAILED]%s\n", RED, RESET);
        return 1;
    }
}
