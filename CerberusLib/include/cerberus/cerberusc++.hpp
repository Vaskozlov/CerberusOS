#ifndef cerberuscpp_hpp
#define cerberuscpp_hpp

#include <stddef.h>
#include <inttypes.h>
#include <type_traits>

#ifndef __BEGIN_DECLS
#  define __BEGIN_DECLS extern "C" {
#  define __END_DECLS }
#endif /* __BEGIN_DECLS */

#if !defined(__cplusplus) && !(__cplusplus >= 201103L)
#  error "Compiler is not suitable"
#endif

#define always_inline __attribute__((always_inline)) inline

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

namespace cerb{

    template<typename _Tp>
    union ByteMask{
        _Tp value;
        u8 mask8[sizeof(_Tp)];
        u16 mask16[sizeof(_Tp) / sizeof(u16)];
        u32 mask32[sizeof(_Tp) / sizeof(u32)];
        u64 mask64[sizeof(_Tp) / sizeof(u64)];
    };

    constexpr double PI = 3.14159265358979323846;

    template<typename _Tp>
    inline _Tp align(_Tp value, u16 powerof2){
        static_assert(std::is_unsigned<_Tp>::value, "Only works for unsigned types");
        return (value & (((_Tp)1 << powerof2) - (_Tp)1)) == (_Tp)0 ? value : value + (((_Tp)1 << powerof2) - (value & (((_Tp)1 << powerof2) - (_Tp)1)));
    }

    template<typename _Tp>
    constexpr unsigned long sizeofbits(){
        return sizeof(_Tp) * 8;
    }

    template<typename _Tp>
    always_inline _Tp ABS(_Tp value){
        static_assert(std::is_signed<_Tp>::value, "Only works for signed types");
        return value < 0 ? -value : value;
    }

    template<typename _Tp>
    always_inline _Tp MAX(_Tp first, _Tp second) { return first > second ? first : second; }

    template<typename _Tp>
    always_inline _Tp MIN(_Tp first, _Tp second) { return first < second ? first : second; }

    template<typename _Tp>
    inline _Tp TO_RAD(_Tp value) { return value / 180 * cerb::PI; }

    template<typename _Tp>
    inline _Tp TO_DEG(_Tp value) { return value * 180 / cerb::PI; }

    constexpr u16 str2u16(const char *__restrict _str){
        return  (static_cast<u16>(_str[1]) << 8) |
                (static_cast<u16>(_str[0]) << 0);
    }

    constexpr u32 str2u32(const char *__restrict _str){
        return  (static_cast<u32>(_str[3]) << 24) |
                (static_cast<u32>(_str[2]) << 16) |
                (static_cast<u32>(_str[1]) << 8 ) |
                (static_cast<u32>(_str[0]) << 0 );
    }

    constexpr u64 str2u64(const char *__restrict _str){
        return  (static_cast<u64>(_str[7]) << 56) |  
                (static_cast<u64>(_str[6]) << 48) |
                (static_cast<u64>(_str[5]) << 40) |
                (static_cast<u64>(_str[4]) << 32) |
                (static_cast<u64>(_str[3]) << 24) |
                (static_cast<u64>(_str[2]) << 16) |
                (static_cast<u64>(_str[1]) << 8 ) |
                (static_cast<u64>(_str[0]) << 0 );
    }
}

#endif /* cerberuscpp_hpp */
