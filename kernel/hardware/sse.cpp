#include "sse.hpp"

u64 SSE::_flags;

void SSE::Scan(){
    auto page1 = ARCH::cpuid(1);

    if (page1.rdx & (1<<26U))   [[likely]] _flags |= SSE_FLAGS::SSE2;
    if (page1.rcx & (1<<0U))    [[likely]] _flags |= SSE_FLAGS::SSE3;
    if (page1.rcx & (1<<19U))   [[likely]] _flags |= SSE_FLAGS::SSE4_1;
    if (page1.rcx & (1<<20U))   [[likely]] _flags |= SSE_FLAGS::SSE4_2;
    if (page1.rcx & (1<< 6U))   [[likely]] _flags |= SSE_FLAGS::SSE4_A;
    if (page1.rcx & (1<<11U))   [[likely]] _flags |= SSE_FLAGS::XOP;
    if (page1.rcx & (1<<16U))   [[likely]] _flags |= SSE_FLAGS::FMA4;
    if (page1.rcx & (1<<29U))   [[likely]] _flags |= SSE_FLAGS::CVT16;
    if (page1.rcx & (1<<28U))   [[likely]] _flags |= SSE_FLAGS::AVX;
    if (page1.rcx & (1<<26U))   [[likely]] _flags |= SSE_FLAGS::XSAVE;

    auto page7_0 = ARCH::cpuid(7, 0);
    if (page7_0.rdx & (1<<26U)) [[likely]] _flags |= SSE_FLAGS::AVX2;
}
