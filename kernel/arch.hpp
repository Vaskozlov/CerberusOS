#ifndef arch_hpp
#define arch_hpp

#if defined(__cplusplus) || defined(c_plusplus)

namespace ARCH{

    #if defined(__x86_64__)
        #include <x86_64/bits/io.h>

        static inline void io_wait(){
            __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
        }

        static inline void Go2Sleep() { while (1) {__asm__ __volatile__ ("hlt");} }

    #endif
    
}

#else

    #if defined(__x86_64__)

     #  define  io_wait outb(0x0, 0x80)

    #endif

#endif

#endif /* arch_hpp */
