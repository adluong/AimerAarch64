#!/bin/bash
# SPDX-License-Identifier: MIT

# Script to preprocess assembly file for different AIMER variants

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <variant> <output_file>"
    echo "  variant: 192s or 192f"
    exit 1
fi

VARIANT=$1
OUTPUT=$2

if [ "$VARIANT" = "192s" ]; then
    # For AIMER-192s: N=256, so loops = 256/8 = 32
    sed -e 's/samsungsds_aimer192f_aarch64_/samsungsds_aimer192s_aarch64_/g' \
        -e 's/MOV    x3, #2/MOV    x3, #32/g' \
        -e 's/MOV    x2, #2/MOV    x2, #32/g' \
        -e 's/MOV    x4, #2/MOV    x4, #32/g' \
        __asm_field.S > "$OUTPUT"
elif [ "$VARIANT" = "192f" ]; then
    # For AIMER-192f: just copy as-is (already has correct values)
    cp __asm_field.S "$OUTPUT"
else
    echo "Error: Unknown variant $VARIANT"
    exit 1
fi
