#ifndef arch_hpp
#define arch_hpp

#include <kernelTypes.h>

__BEGIN_DECLS

void memset8 (void *__address, u8  __value, size_t __times);
void memset16(void *__address, u16 __value, size_t __times);
void memset32(void *__address, u32 __value, size_t __times);
void memset64(void *__address, u64 __value, size_t __times);

void memcpy8  (void *__dest, const void *__src, size_t __times);
void memcpy16 (void *__dest, const void *__src, size_t __times);
void memcpy32 (void *__dest, const void *__src, size_t __times);
void memcpy64 (void *__dest, const void *__src, size_t __times);

__END_DECLS

namespace ARCH{

    #include <x86_64/bits/io.h>

    always_inline static void io_wait(){
        __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
    }
    
    always_inline static void hlt() {
        __asm__ __volatile__ ("hlt");
    }

    always_inline static void Go2Sleep() {
        while (1) { hlt();}
    }

    always_inline static u8 rotate_left8 (u8 u, u8 r)
    {
        __asm__ ("rolb %%cl, %0" : "+r" (u) : "c" (r));
        return u;
    }

    always_inline static u16 rotate_left16 (u16 u, u8 r)
    {
        __asm__ ("rolw %%cl, %0" : "+r" (u) : "c" ((u16)r));
        return u;
    }

    always_inline static u32 rotate_left32(u32 u, u8 r)
    {
        __asm__ ("roll %%cl, %0" : "+r" (u) : "c" (r));
        return u;
    }

    always_inline static u64 rotate_left64(u64 u, u8 r)
    {
        __asm__ ("rolq %%cl, %0" : "+r" (u) : "c" (r));
        return u;
    }

    always_inline static void memset8(void *__address, u8 __value, size_t __times){
        ::memset8(__address, __value, __times);
    }

    always_inline static void memset64(void *__address, u16 __value, size_t __times){
        ::memset16(__address, __value, __times);
    }

    always_inline static void memset32(void *__address, u32 __value, size_t __times){
        ::memset32(__address, __value, __times);
    }

    always_inline static void memset64(void *__address, u64 __value, size_t __times){
        ::memset64(__address, __value, __times);
    }
}


#endif /* arch_hpp */
