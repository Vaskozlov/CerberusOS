#ifndef arch_hpp
#define arch_hpp

#include <kernelTypes.h>

__BEGIN_DECLS

typedef struct{
    u64 rax;
    u64 rcx;
    u64 rdx;
} __attribute__((packed)) CPUID_RESULT_t;

void memset8 (void *__address, u8  __value, size_t __times);
void memset16(void *__address, u16 __value, size_t __times);
void memset32(void *__address, u32 __value, size_t __times);
void memset64(void *__address, u64 __value, size_t __times);

void memcpy8  (void *__dest, const void *__src, size_t __times);
void memcpy16 (void *__dest, const void *__src, size_t __times);
void memcpy32 (void *__dest, const void *__src, size_t __times);
void memcpy64 (void *__dest, const void *__src, size_t __times);

void EnableSSEIN();
void EnableAVXIN();
CPUID_RESULT_t CPU_ID(u64 page, u64 subpage);

__END_DECLS

#if defined(__cplusplus)

namespace ARCH{

    #include <x86_64/bits/io.h>

    always_inline CPUID_RESULT_t cpuid(u64 page, u64 subpage = 0){
        return CPU_ID(page, subpage);
    }

    always_inline static void io_wait(){
        __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
    }
    
    always_inline static void hlt() {
        __asm__ __volatile__ ("hlt");
    }

    always_inline static void enableINT(){
        __asm__ __volatile__ ("sti");
    }

    always_inline static void disableINT(){
        __asm__ __volatile__ ("cli");
    }

    always_inline static void Go2Sleep() {
        while (1) { hlt();}
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

#endif /* __cplusplus */ 

#endif /* arch_hpp */
