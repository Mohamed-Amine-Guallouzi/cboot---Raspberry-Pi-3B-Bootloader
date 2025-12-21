/******************************************************************************
 * @file    types.h
 * @author  Mohammed Amine
 * @brief   Standard type definitions for bare-metal programming
 *
 * Provides fixed-width integer types and other common type definitions
 * without relying on standard library headers.
 ******************************************************************************/

#ifndef TYPES_H
#define TYPES_H

/* ============================================================================
 * Fixed-width Integer Types
 * ============================================================================ */

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;

typedef signed long long int64_t;
typedef unsigned long long uint64_t;

/* ============================================================================
 * Architecture-dependent Types
 * ============================================================================ */

/* ARM64 specific - 64-bit pointers */
typedef uint64_t uintptr_t;
typedef uint64_t size_t;
typedef int64_t ssize_t;

/* ============================================================================
 * Boolean Type
 * ============================================================================ */

typedef enum {
    false = 0,
    true = 1
} bool;

/* ============================================================================
 * Common Macros
 * ============================================================================ */

#define NULL ((void*)0)
#define offsetof(type, member) ((size_t)&((type*)0)->member)

/* Memory barriers for ARM64 */
#define mb()    asm volatile("dsb sy" ::: "memory")
#define rmb()   asm volatile("dsb ld" ::: "memory")
#define wmb()   asm volatile("dsb st" ::: "memory")

/* Compiler built-ins */
#define nop()   asm volatile("nop")
#define wfi()   asm volatile("wfi")

#endif /* TYPES_H */