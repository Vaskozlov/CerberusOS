#ifndef io_hpp
#define io_hpp

#include <x86_64/bits/io.h>

inline void io_wait(){
    __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
}

#endif /* io_hpp */
