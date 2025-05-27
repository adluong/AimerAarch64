// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Include both variants' headers
#include "aimer128s/api.h"
#include "aimer128s/params.h"
#include "aimer128f/api.h"
#include "aimer128f/params.h"

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

// Function declarations for AIMER-128s
extern int samsungsds_aimer128s_aarch64_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int samsungsds_aimer128s_aarch64_crypto_sign(uint8_t *sm, size_t *smlen,
        const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int samsungsds_aimer128s_aarch64_crypto_sign_signature(uint8_t *sig, size_t *siglen,
        const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int samsungsds_aimer128s_aarch64_crypto_sign_verify(const uint8_t *sig, size_t siglen,
        const uint8_t *m, size_t mlen, const uint8_t *pk);
extern int samsungsds_aimer128s_aarch64_crypto_sign_open(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen, const uint8_t *pk);

// Function declarations for AIMER-128f
extern int samsungsds_aimer128f_aarch64_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int samsungsds_aimer128f_aarch64_crypto_sign(uint8_t *sm, size_t *smlen,
        const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int samsungsds_aimer128f_aarch64_crypto_sign_signature(uint8_t *sig, size_t *siglen,
        const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int samsungsds_aimer128f_aarch64_crypto_sign_verify(const uint8_t *sig, size_t siglen,
        const uint8_t *m, size_t mlen, const uint8_t *pk);
extern int samsungsds_aimer128f_aarch64_crypto_sign_open(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen, const uint8_t *pk);

// Test results structure
typedef struct {
    int keypair_success;
    int sign_success;
    int verify_success;
    int tamper_detection;
    double avg_keypair_time;
    double avg_sign_time;
    double avg_verify_time;
} test_results_t;

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

// Test AIMER-128s
test_results_t test_aimer128s() {
    test_results_t results = {0};
    
    printf("\n%s========== Testing AIMER-128s ==========%s\n", BLUE, RESET);
    printf("Algorithm: aimer128s\n");
    printf("Public key size: %d bytes\n", CRYPTO_PUBLICKEYBYTES);
    printf("Secret key size: %d bytes\n", CRYPTO_SECRETKEYBYTES);
    printf("Signature size: %d bytes\n", 4160);  // AIMER-128s signature size
    
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    
    // Test key generation
    printf("\n%sTesting key generation...%s\n", YELLOW, RESET);
    double keypair_time = 0;
    
    for (int i = 0; i < TIMING_ITERATIONS; i++) {
        double start = get_time();
        int ret = samsungsds_aimer128s_aarch64_crypto_sign_keypair(pk, sk);
        double end = get_time();
        keypair_time += (end - start);
        
        if (ret != 0) {
            printf("%s[FAIL]%s Key generation failed\n", RED, RESET);
            return results;
        }
    }
    
    results.keypair_success = 1;
    results.avg_keypair_time = keypair_time / TIMING_ITERATIONS;
    printf("%s[PASS]%s Key generation successful\n", GREEN, RESET);
    printf("Average time: %.2f μs\n", results.avg_keypair_time);
    
    // Test signing and verification
    int msg_sizes[] = MESSAGE_SIZES;
    printf("\n%sTesting signing and verification...%s\n", YELLOW, RESET);
    
    results.sign_success = 1;
    results.verify_success = 1;
    results.tamper_detection = 1;
    
    double sign_time = 0;
    double verify_time = 0;
    
    for (int i = 0; i < TEST_MESSAGES; i++) {
        size_t msg_size = msg_sizes[i];
        uint8_t* msg = malloc(msg_size);
        uint8_t* sm = malloc(msg_size + 4160);
        uint8_t* msg2 = malloc(msg_size);
        size_t smlen, mlen2;
        
        // Generate random message
        for (size_t j = 0; j < msg_size; j++) {
            msg[j] = rand() & 0xFF;
        }
        
        printf("\nMessage %d (size: %zu bytes)\n", i+1, msg_size);
        
        // Test signing
        double start = get_time();
        int ret = samsungsds_aimer128s_aarch64_crypto_sign(sm, &smlen, msg, msg_size, sk);
        double end = get_time();
        sign_time += (end - start);
        
        if (ret != 0 || smlen != msg_size + 4160) {
            printf("%s[FAIL]%s Signing failed\n", RED, RESET);
            results.sign_success = 0;
            free(msg); free(sm); free(msg2);
            continue;
        }
        
        // Test verification
        start = get_time();
        ret = samsungsds_aimer128s_aarch64_crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
        end = get_time();
        verify_time += (end - start);
        
        if (ret != 0 || mlen2 != msg_size || memcmp(msg, msg2, msg_size) != 0) {
            printf("%s[FAIL]%s Verification failed\n", RED, RESET);
            results.verify_success = 0;
        } else {
            printf("%s[PASS]%s Message signed and verified correctly\n", GREEN, RESET);
        }
        
        // Test tamper detection
        sm[msg_size/2] ^= 0x01;
        ret = samsungsds_aimer128s_aarch64_crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
        if (ret == 0) {
            printf("%s[FAIL]%s Message tampering not detected\n", RED, RESET);
            results.tamper_detection = 0;
        }
        sm[msg_size/2] ^= 0x01; // Restore
        
        // Tamper with signature
        sm[msg_size + 2080] ^= 0x01;  // Middle of signature
        ret = samsungsds_aimer128s_aarch64_crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
        if (ret == 0) {
            printf("%s[FAIL]%s Signature tampering not detected\n", RED, RESET);
            results.tamper_detection = 0;
        }
        
        free(msg); free(sm); free(msg2);
    }
    
    results.avg_sign_time = sign_time / TEST_MESSAGES;
    results.avg_verify_time = verify_time / TEST_MESSAGES;
    
    printf("\n%sPerformance Summary:%s\n", YELLOW, RESET);
    printf("Average signing time: %.2f μs\n", results.avg_sign_time);
    printf("Average verification time: %.2f μs\n", results.avg_verify_time);
    
    return results;
}

// Test AIMER-128f
test_results_t test_aimer128f() {
    test_results_t results = {0};
    
    printf("\n%s========== Testing AIMER-128f ==========%s\n", BLUE, RESET);
    printf("Algorithm: aimer128f\n");
    printf("Public key size: %d bytes\n", CRYPTO_PUBLICKEYBYTES);
    printf("Secret key size: %d bytes\n", CRYPTO_SECRETKEYBYTES);
    printf("Signature size: %d bytes\n", 5888);  // AIMER-128f signature size
    
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    
    // Test key generation
    printf("\n%sTesting key generation...%s\n", YELLOW, RESET);
    double keypair_time = 0;
    
    for (int i = 0; i < TIMING_ITERATIONS; i++) {
        double start = get_time();
        int ret = samsungsds_aimer128f_aarch64_crypto_sign_keypair(pk, sk);
        double end = get_time();
        keypair_time += (end - start);
        
        if (ret != 0) {
            printf("%s[FAIL]%s Key generation failed\n", RED, RESET);
            return results;
        }
    }
    
    results.keypair_success = 1;
    results.avg_keypair_time = keypair_time / TIMING_ITERATIONS;
    printf("%s[PASS]%s Key generation successful\n", GREEN, RESET);
    printf("Average time: %.2f μs\n", results.avg_keypair_time);
    
    // Test signing and verification
    int msg_sizes[] = MESSAGE_SIZES;
    printf("\n%sTesting signing and verification...%s\n", YELLOW, RESET);
    
    results.sign_success = 1;
    results.verify_success = 1;
    results.tamper_detection = 1;
    
    double sign_time = 0;
    double verify_time = 0;
    
    for (int i = 0; i < TEST_MESSAGES; i++) {
        size_t msg_size = msg_sizes[i];
        uint8_t* msg = malloc(msg_size);
        uint8_t* sm = malloc(msg_size + 5888);
        uint8_t* msg2 = malloc(msg_size);
        size_t smlen, mlen2;
        
        // Generate random message
        for (size_t j = 0; j < msg_size; j++) {
            msg[j] = rand() & 0xFF;
        }
        
        printf("\nMessage %d (size: %zu bytes)\n", i+1, msg_size);
        
        // Test signing
        double start = get_time();
        int ret = samsungsds_aimer128f_aarch64_crypto_sign(sm, &smlen, msg, msg_size, sk);
        double end = get_time();
        sign_time += (end - start);
        
        if (ret != 0 || smlen != msg_size + 5888) {
            printf("%s[FAIL]%s Signing failed\n", RED, RESET);
            results.sign_success = 0;
            free(msg); free(sm); free(msg2);
            continue;
        }
        
        // Test verification
        start = get_time();
        ret = samsungsds_aimer128f_aarch64_crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
        end = get_time();
        verify_time += (end - start);
        
        if (ret != 0 || mlen2 != msg_size || memcmp(msg, msg2, msg_size) != 0) {
            printf("%s[FAIL]%s Verification failed\n", RED, RESET);
            results.verify_success = 0;
        } else {
            printf("%s[PASS]%s Message signed and verified correctly\n", GREEN, RESET);
        }
        
        // Test tamper detection
        sm[msg_size/2] ^= 0x01;
        ret = samsungsds_aimer128f_aarch64_crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
        if (ret == 0) {
            printf("%s[FAIL]%s Message tampering not detected\n", RED, RESET);
            results.tamper_detection = 0;
        }
        sm[msg_size/2] ^= 0x01; // Restore
        
        // Tamper with signature
        sm[msg_size + 2944] ^= 0x01;  // Middle of signature
        ret = samsungsds_aimer128f_aarch64_crypto_sign_open(msg2, &mlen2, sm, smlen, pk);
        if (ret == 0) {
            printf("%s[FAIL]%s Signature tampering not detected\n", RED, RESET);
            results.tamper_detection = 0;
        }
        
        free(msg); free(sm); free(msg2);
    }
    
    results.avg_sign_time = sign_time / TEST_MESSAGES;
    results.avg_verify_time = verify_time / TEST_MESSAGES;
    
    printf("\n%sPerformance Summary:%s\n", YELLOW, RESET);
    printf("Average signing time: %.2f μs\n", results.avg_sign_time);
    printf("Average verification time: %.2f μs\n", results.avg_verify_time);
    
    return results;
}

// Print comparison results
void print_comparison(test_results_t res_128s, test_results_t res_128f) {
    printf("\n%s========== Performance Comparison ==========%s\n", BLUE, RESET);
    printf("                    AIMER-128s    AIMER-128f\n");
    printf("Key generation:     %.2f μs     %.2f μs\n", 
           res_128s.avg_keypair_time, res_128f.avg_keypair_time);
    printf("Signing:            %.2f μs     %.2f μs\n", 
           res_128s.avg_sign_time, res_128f.avg_sign_time);
    printf("Verification:       %.2f μs     %.2f μs\n", 
           res_128s.avg_verify_time, res_128f.avg_verify_time);
    
    printf("\nSpeedup factors:\n");
    printf("Verification: AIMER-128f is %.2fx %s than AIMER-128s\n",
           res_128s.avg_verify_time / res_128f.avg_verify_time,
           res_128s.avg_verify_time > res_128f.avg_verify_time ? "faster" : "slower");
}

int main() {
    printf("%s╔══════════════════════════════════════════╗%s\n", BLUE, RESET);
    printf("%s║     AIMER Digital Signature Test Suite    ║%s\n", BLUE, RESET);
    printf("%s╚══════════════════════════════════════════╝%s\n", BLUE, RESET);
    
    // Initialize random seed
    srand(time(NULL));
    
    // Test both variants
    test_results_t res_128s = test_aimer128s();
    test_results_t res_128f = test_aimer128f();
    
    // Print comparison
    print_comparison(res_128s, res_128f);
    
    // Final test summary
    printf("\n%s========== Test Summary ==========%s\n", BLUE, RESET);
    int all_passed = 1;
    
    printf("AIMER-128s: ");
    if (res_128s.keypair_success && res_128s.sign_success && 
        res_128s.verify_success && res_128s.tamper_detection) {
        printf("%s[ALL TESTS PASSED]%s\n", GREEN, RESET);
    } else {
        printf("%s[SOME TESTS FAILED]%s\n", RED, RESET);
        all_passed = 0;
    }
    
    printf("AIMER-128f: ");
    if (res_128f.keypair_success && res_128f.sign_success && 
        res_128f.verify_success && res_128f.tamper_detection) {
        printf("%s[ALL TESTS PASSED]%s\n", GREEN, RESET);
    } else {
        printf("%s[SOME TESTS FAILED]%s\n", RED, RESET);
        all_passed = 0;
    }
    
    return all_passed ? 0 : 1;
}
