#ifndef arch_hpp
#define arch_hpp

#include <kernelTypes.h>

#if defined(__cplusplus) || defined(c_plusplus)

namespace ARCH{

    #if defined(__x86_64__)
        #include <x86_64/bits/io.h>

        strict_inline static void io_wait(){
            __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
        }
        
        strict_inline static void hlt() {
            __asm__ __volatile__ ("hlt");
        }

        strict_inline static void Go2Sleep() {
            while (1) { hlt();}
        }

        strict_inline static u8 rotate_left (u8 u, size_t r)
        {
            __asm__ ("rolw %%cl, %0" : "+r" (u) : "c" (r));
            return u;
        }

        strict_inline static u16 rotate_left (u16 u, size_t r)
        {
            __asm__ ("rolw %1, %0" : "+rax" (u) : "rdi" (r));
            return u;
        }

        strict_inline static u32 rotate_left (u32 u, size_t r)
        {
            __asm__ ("roll %%cl, %0" : "+r" (u) : "c" (r));
            return u;
        }

        strict_inline static u64 rotate_left (u64 u, size_t r)
        {
            __asm__ ("rolq %%cl, %0" : "+r" (u) : "c" (r));
            return u;
        }

    #endif
}

#else

    #if defined(__x86_64__)

     #  define  io_wait outb(0x0, 0x80)

    #endif

#endif

#endif /* arch_hpp */
