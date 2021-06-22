#include <optlib.h>

#define SSE_XMM_SIZE 16

const void * memclr_sse2(const void * const m_start, const size_t m_count){
    // "i" is our counter of how many bytes we've cleared
    size_t i;

    if (m_count == 0) return m_start;

    // find out if "m_start" is aligned on a SSE_XMM_SIZE boundary
    if((size_t)m_start & (SSE_XMM_SIZE - 1))
    {
        i = 0;

        // we need to clear byte-by-byte until "m_start" is aligned on an SSE_XMM_SIZE boundary
        // ... and lets make sure we don't copy 'too' many bytes (i < m_count)
        while(((size_t)m_start + i) & (SSE_XMM_SIZE - 1) && i < m_count)
        {
            asm("stosb;" :: "D"((size_t)m_start + i), "a"(0));
            i++;
        }
    }
    else
    {
        // if "m_start" was aligned, set our count to 0
        i = 0;
    }
 
    // clear 64-byte chunks of memory (4 16-byte operations)
    for(; i + 64 <= m_count; i += 64)
    {
        asm volatile(" pxor %%xmm0, %%xmm0;	"    // set XMM0 to 0
                     " movdqa %%xmm0, 0(%0);	"    // move 16 bytes from XMM0 to %0 + 0
                     " movdqa %%xmm0, 16(%0);	"
                     " movdqa %%xmm0, 32(%0);	"
                     " movdqa %%xmm0, 48(%0);	"
                     :: "r"((size_t)m_start + i));
    }
 
    // copy the remaining bytes (if any)
    asm(" rep stosb; " :: "a"((size_t)(0)), "D"(((size_t)m_start) + i), "c"(m_count - i));

    // "i" will contain the total amount of bytes that were actually transfered
    i += m_count - i;

    // we return "m_start" + the amount of bytes that were transfered
    return (void *)(((size_t)m_start) + i);
}

size_t strlen_sse4_2(const char * const str){
   size_t index;

   asm(" mov $-16, %0;                      "
       " pxor %%xmm0, %%xmm0;               "
       ".strlen_4_2_start:                  "
       " add $16, %0;                       "
       " pcmpistri $0x08, (%0,%1), %%xmm0;  "
       " jnz .strlen_4_2_start;             "
       " add %2, %0;                        "
       :"=a"(index):"d"((size_t)str),"c"((size_t)str));
 
    return index;
}

void paddb(uint8_t DST[16], const uint8_t v2[16]){
    // we need an aligned buffer, just in case
    uint8_t SRC[16] __attribute__((aligned(16)));

    // if SRC is not aligned, we need to copy those values into our temp buffer
    if((size_t)SRC & (SSE_XMM_SIZE - 1))
    { memcpy64(SRC, v2, 16 / sizeof(u64)); }

    // if SRC is not aligned, PADDB will GPF
    asm(" movdqu (%0), %%xmm1;   "   // move DST into XMM1
        " paddb (%1), %%xmm1;    "   // add DST+SRC into XMM1
        " movdqu %%xmm1, (%0);   "   // store XMM1 into DST
        :: "r"((size_t)DST), "r"((size_t)SRC));
}

void paddw(uint16_t DST[8], const uint16_t v2[8]){
    // we need an aligned buffer, just in case
    uint16_t SRC[8] __attribute__((aligned(16)));

    // if SRC is not aligned, we need to copy those values into our temp buffer
    if((size_t)SRC & (SSE_XMM_SIZE - 1))
    { memcpy64(SRC, v2, 16 / sizeof(u64)); }

    // if SRC is not aligned, PADDB will GPF
    asm(" movdqu (%0), %%xmm1;   "   // move DST into XMM1
        " paddw (%1), %%xmm1;    "   // add DST+SRC into XMM1
        " movdqu %%xmm1, (%0);   "   // store XMM1 into DST
        :: "r"((size_t)DST), "r"((size_t)SRC));
}

void paddd(uint32_t DST[4], const uint32_t v2[4]){
    // we need an aligned buffer, just in case
    uint32_t SRC[4] __attribute__((aligned(16)));

    // if SRC is not aligned, we need to copy those values into our temp buffer
    if((size_t)SRC & (SSE_XMM_SIZE - 1))
    { memcpy64(SRC, v2, 16 / sizeof(u64)); }

    // if SRC is not aligned, PADDB will GPF
    asm(" movdqu (%0), %%xmm1;   "   // move DST into XMM1
        " paddd (%1), %%xmm1;    "   // add DST+SRC into XMM1
        " movdqu %%xmm1, (%0);   "   // store XMM1 into DST
        :: "r"((size_t)DST), "r"((size_t)SRC));
}

void paddq(uint64_t DST[2], const uint64_t v2[2]){
    // we need an aligned buffer, just in case
    uint64_t SRC[2] __attribute__((aligned(16)));

    // if SRC is not aligned, we need to copy those values into our temp buffer
    if((size_t)SRC & (SSE_XMM_SIZE - 1))
    { memcpy64(SRC, v2, 16 / sizeof(u64)); }

    // if SRC is not aligned, PADDB will GPF
    asm(" movdqu (%0), %%xmm1;   "   // move DST into XMM1
        " paddq (%1), %%xmm1;    "   // add DST+SRC into XMM1
        " movdqu %%xmm1, (%0);   "   // store XMM1 into DST
        :: "r"((size_t)DST), "r"((size_t)SRC));
}
