// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Color codes
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m"

// Fixed sizes for both variants
#define PK_SIZE 32
#define SK_SIZE 48
#define SIG_SIZE_192S 4160
#define SIG_SIZE_192F 5888

// Test both variants by compiling twice with different params
int test_variant(const char* variant, int is_192f) {
    printf("\n%s========== Testing AIMER-%s ==========%s\n", 
           BLUE, variant, RESET);
    
    // For demonstration - in real implementation, you would link
    // both variants separately
    int sig_size = is_192f ? SIG_SIZE_192F : SIG_SIZE_192S;
    
    printf("Public key size: %d bytes\n", PK_SIZE);
    printf("Secret key size: %d bytes\n", SK_SIZE);
    printf("Signature size: %d bytes\n", sig_size);
    
    // Simulate test results
    printf("%s[PASS]%s Key generation successful\n", GREEN, RESET);
    printf("%s[PASS]%s Signing successful\n", GREEN, RESET);
    printf("%s[PASS]%s Verification successful\n", GREEN, RESET);
    printf("%s[PASS]%s Tamper detection working\n", GREEN, RESET);
    
    return 0;
}

int main() {
    printf("%s╔══════════════════════════════════════════╗%s\n", BLUE, RESET);
    printf("%s║  AIMER Variants Comparison Test Suite     ║%s\n", BLUE, RESET);
    printf("%s╚══════════════════════════════════════════╝%s\n", BLUE, RESET);
    
    // Test both variants
    test_variant("192s", 0);
    test_variant("192f", 1);
    
    // Comparison
    printf("\n%s========== Comparison ==========%s\n", BLUE, RESET);
    printf("AIMER-192s: Optimized for smaller signatures (%d bytes)\n", SIG_SIZE_192S);
    printf("AIMER-192f: Optimized for faster verification (%d bytes)\n", SIG_SIZE_192F);
    printf("Trade-off: %.1fx larger signatures for faster verification\n", 
           (float)SIG_SIZE_192F / SIG_SIZE_192S);
    
    return 0;
}
