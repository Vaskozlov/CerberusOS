#ifndef optlib_h
#define optlib_h

/*
    https://wiki.osdev.org/User:01000101/optlib/
*/

#include <arch.hpp>
#include <cerberus/cerberus.h>

__BEGIN_DECLS

void paddb(uint8_t DST[16], const uint8_t v2[16]);
void paddw(uint16_t DST[8], const uint16_t v2[8]);
void paddd(uint32_t DST[4], const uint32_t v2[4]);
void paddq(uint64_t DST[2], const uint64_t v2[2]);

size_t strlen_sse4_2(const char * const str);
const void * memclr_sse2(const void * const m_start, const size_t m_count);

__END_DECLS

#endif /* optlib_h */
