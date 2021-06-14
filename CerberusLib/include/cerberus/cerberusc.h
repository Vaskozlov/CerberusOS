#if !defined(cerberusc_h) && !defined(cerberuscpp_hpp)
#define cerberusc_h

#include <stddef.h>
#include <inttypes.h>

#ifndef __BEGIN_DECLS
#  define __BEGIN_DECLS
#  define __END_DECLS
#endif /* __BEGIN_DECLS */

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

#define CERB_PI 3.14159265358979323846

#define TO_RAD(x) (x / 180 * CERB_PI)
#define TO_DEG(x) (x * 180 * CERB_PI)

#define align(value, powerof2) ((value & ((1 << powerof2) - 1)) == 0 ? value : value + (1 << powerof2) - (value & ((1 << powerof2) - 1)))

#ifndef MAX
#    define MAX(a, b) ((a) > (b) ? (a) : (b))
#    define MIN(a, b) ((a) < (b) ? (a) : (b))
#  endif

#define str2u16(_str) ((((u16)_str[1]) << 8 ) | ((u16)_str[0]))
#define str2u32(_str) ((((u32)_str[3]) << 24) | (((u32)_str[2]) << 16) | (((u32)_str[1]) << 8)  | ((u32)_str[0]))
#define str2u64(_str) ((((u64)_str[7]) << 56) | (((u64)_str[6]) << 48) | (((u64)_str[5]) << 40) | (((u64)_str[4]) << 32) | (((u64)_str[3]) << 24) | (((u64)_str[2]) << 16) | (((u64)_str[1]) << 8) | ((u64)_str[0]))

#define ABS(value) ((value) < 0 ? -(value) : (value))
#define sizeofbits(x) (sizeof(x) * 8)

#endif /* cerberusc_h */
