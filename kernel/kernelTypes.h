#ifndef kernelTypes_h
#define kernelTypes_h

#include <stddef.h>
#include <inttypes.h>

#if defined(__cplusplus) && !defined(__BEGIN_DECL)
#  define __BEGIN_DECL extern "C" {
#  define __END_DECL }
#elif !defined(__BEGIN_DECL)
#  define __BEGIN_DECL
#  define __END_DECL
#endif /* __cplusplus */

#if defined(__clang__)
#  define COMPILER_NAME "clang"
#elif defined(__GNUC__)
#  define COMPILER_NAME "gcc"
#endif

#if defined(MAJORE_VERSION)
#  define CERBERUS_MAJORE_VERSION MAJORE_VERSION
#else
#  define CERBERUS_MAJORE_VERSION 0
#endif

#if defined(MINORE_VERSION)
#  define CERBERUS_MINORE_VERSION MINORE_VERSION
#else
#  define CERBERUS_MINORE_VERSION 0
#endif

#define strict_inline __attribute__((always_inline)) inline

/**
 * str2u16 - converts array of char to u16
 * @str: array of char
 * @str_offset: offset of first element
 * Return: 16 bit value from array of char with offset
 */
#define str2u16(str, str_offset) \
        (\
          ((u16)str[str_offset + 1] << 8)\
        | ((u16)str[str_offset])\
        )

/**
 * str2u32 - converts array of char to u32
 * @str: array of char
 * @str_offset: offset of first element
 * Return: 32 bit value from array of char with offset
 */
#define str2u32(str, str_offset) \
        (\
          ((u32)str[str_offset + 3] << 24)\
        | ((u32)str[str_offset + 2] << 16)\
        | ((u32)str[str_offset + 1] << 8)\
        | ((u32)str[str_offset])\
        )

/**
 * 64 - converts array of char to 64
 * @str: array of char
 * @str_offset: offset of first element
 * Return: 64 bit value from array of char with offset
 */
#define str2u64(str, str_offset) \
        (\
          ((u64)str[str_offset + 7] << 56)\
        | ((u64)str[str_offset + 6] << 48)\
        | ((u64)str[str_offset + 5] << 40)\
        | ((u64)str[str_offset + 4] << 32)\
        | ((u64)str[str_offset + 3] << 24)\
        | ((u64)str[str_offset + 2] << 16)\
        | ((u64)str[str_offset + 1] << 8)\
        | ((u64)str[offstr_offsetset])\
        )

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

#if defined(__cplusplus)

    template<class T>
    strict_inline T align(T number, size_t power)
    {
        auto middle = (number & ((1UL << power) - 1));
        middle = middle == 0 ? (1UL << power) : middle;
        return number + (1UL << power) - middle;
    }

    template<typename T>
    struct ByteMask{
        T value;
        u8 mask[sizeof(T)];
    };

    #define _MAX 1

    template<typename T>
    strict_inline T MAX(T a, T b) { return a > b ? a : b; }

    template<typename T>
    strict_inline T MIN(T a, T b) { return a < b ? a : b; }


#else
#  ifndef _MAX
#    define MAX(a, b) ((a) > (b) ? (a) : (b))
#    define MIN(a, b) ((a) < (b) ? (a) : (b))
#    define _MAX 1
#  endif
#endif

#endif /* kernelTypes_h */
