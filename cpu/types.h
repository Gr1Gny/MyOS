#ifndef TYPES_H
#define TYPES_H

/* Instead of using 'chars' to allocate non-character bytes,
 * we will use these new types with no semantic meaning */
typedef unsigned int   u32;
typedef          int   s32;
typedef unsigned short u16;
typedef          short s16;
typedef unsigned char  u8;
typedef          char  s8;

/* Additional useful types for OS development */
typedef u32 size_t;     /* For sizes and lengths - 32-bit is appropriate for 32-bit OS */

/* Boolean constants - use with u8 type for boolean values */
#define true  1
#define false 0

/* NULL pointer */
#define NULL ((void*)0)

/* Common bit masks */
#define BYTE_MASK 0xFF
#define WORD_MASK 0xFFFF
#define BITS_PER_BYTE 8

/* Bit manipulation macros */
#define low_16(address) (u16)((address) & WORD_MASK)
#define high_16(address) (u16)(((address) >> 16) & WORD_MASK)
#define low_8(value) (u8)((value) & BYTE_MASK)
#define high_8(value) (u8)(((value) >> BITS_PER_BYTE) & BYTE_MASK)

/* Utility macros */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif
