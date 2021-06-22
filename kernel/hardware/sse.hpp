#ifndef sse_hpp
#define sse_hpp

#include <kernel.h>
#include <arch.hpp>

enum SSE_FLAGS : u32 {
    SSE2    = 1<<0,
    SSE3    = 1<<1,
    SSE4_1  = 1<<2,
    SSE4_2  = 1<<3,
    SSE4_A  = 1<<4,
    XOP     = 1<<5,
    FMA4    = 1<<6,
    CVT16   = 1<<7,
    AVX     = 1<<8,
    XSAVE   = 1<<9,
    AVX2    = 1<<10,
    AVX512  = 1<<11 // CerberusOS will not support AVX512
};

class SSE{
    static u64 _flags;

public:
    static always_inline auto flags() -> u64{
        return _flags;
    }

    static always_inline auto FlagPresent(SSE_FLAGS flag) -> bool{
        return (_flags & flag) > 0;
    }

public:
    static always_inline void enableSSE() { EnableSSEIN(); }
    static void Scan();
};

#endif /* sse_hpp */
