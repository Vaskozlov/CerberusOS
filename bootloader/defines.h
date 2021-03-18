#ifndef defines_h
#define defines_h

#include <inttypes.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

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

#endif /* defines_h */
