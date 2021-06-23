#include "Interrupts.h"
#include <cerberus/printf.h>
#include "scheduling/pit/pit.hpp"
#include <memory/VMManager.hpp>
#include <memory/kmalloc.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

extern VMManager KernelVMM;


__attribute__((interrupt))
void DevideByZero_Handler_SSE       /* 0x00 */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void Debug_Handler_SSE              /* 0x01 */  (struct interrupt_frame *frame){
    cerbPrintString("Debug interrupt!\n");
}

__attribute__((interrupt))
void Breakpoint_Handler_SSE         /* 0x03 */  (struct interrupt_frame *frame){
    cerbPrintString("Breakpoint interrupt!\n");
}

__attribute__((interrupt))
void Overflow_Handler_SSE           /* 0x05 */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void BoundRange_Handler_SSE         /* 0x04 */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void InvalidOpcode_Handler_SSE      /* 0x06 */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void DeviceNotAvailable_Handler_SSE /* 0x07 */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void DoubleFault_Handler_SSE        /* 0x08 */  (struct interrupt_frame *frame, uword_t error_code){
    cerbPrintf("Interrupt %s with error %#b (%x) at line %p\n", __FUNCTION__, error_code, error_code, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void InvalidTSS_Handler_SSE         /* 0x0A */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void SegmentNotPresent_Handler_SSE  /* 0x0B */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void StackSegment_Handler_SSE       /* 0x0C */  (struct interrupt_frame *frame, uword_t error_code){
    cerbPrintf("Interrupt %s with error %#b (%x) at line %p\n", __FUNCTION__, error_code, error_code, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void GeneralProtection_Handler_SSE  /* 0x0D */  (struct interrupt_frame *frame, uword_t error_code){
    cerbPrintf("Interrupt %s with error %#b (%x) at line %p\n", __FUNCTION__, error_code, error_code, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt)) 
void PageFault_Handler_SSE          /* 0x0E */  (struct interrupt_frame *frame, uword_t error_code){
    uintmax_t memoryRegion;

    __asm__ __volatile__(
        "movq %%cr2, %0\n"
        : "=r" (memoryRegion)
    );
   
    if (memoryRegion != 0x0 && memoryRegion >= MallocMainHeder.MallocBegin && memoryRegion < MallocMainHeder.MallocHead){
        auto page = PA::Get2MB();
        KernelVMM.MapMemory2MB((void*)(memoryRegion - (memoryRegion & ((1<<21UL) - 1))), page);
    }
    else{
        cerbPrintf("PANIC!!! Page fault with error %zu. Target address was %p. At line %p\n", error_code, memoryRegion, frame->ip);
        ARCH::Go2Sleep();
    }

    return;
}

__attribute__((interrupt))
void AlignmentCheck_Handler_SSE     /* 0x11 */  (struct interrupt_frame *frame, uword_t error_code){
    cerbPrintf("Interrupt %s with error %#b (%x) at line %p\n", __FUNCTION__, error_code, error_code, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void MachineCheck_Handler_SSE       /* 0x12 */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void SIMD_Handler_SSE               /* 0x13 */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void Virtualization_Handler_SSE     /* 0x14 */  (struct interrupt_frame *frame){
    cerbPrintf("Interrupt %s at line %p\n", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt)) 
void Pit_Handler_SSE                /* 0x20 */  (struct interrupt_frame *frame){
    PIT::Tick();
    PIC_EndMaster();
}

__attribute__((interrupt)) 
void EmptyIQR_Handler_SSE           /* ANY IRQ */ (struct interrupt_frame *frame){
    PIC_EndMaster();
    return;
}

void RemapPIC(){
    u8 a1, a2; 

    a1 = ARCH::inb(PIC1_DATA);
    ARCH::io_wait();
    a2 = ARCH::inb(PIC2_DATA);
    ARCH::io_wait();

    ARCH::outb(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND);
    ARCH::io_wait();
    ARCH::outb(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND);
    ARCH::io_wait();

    ARCH::outb(0x20, PIC1_DATA);
    ARCH::io_wait();
    ARCH::outb(0x28, PIC2_DATA);
    ARCH::io_wait();

    ARCH::outb(4, PIC1_DATA);
    ARCH::io_wait();
    ARCH::outb(2, PIC2_DATA);
    ARCH::io_wait();

    ARCH::outb(ICW4_8086, PIC1_DATA);
    ARCH::io_wait();
    ARCH::outb(ICW4_8086, PIC2_DATA);
    ARCH::io_wait();

    ARCH::outb(a1, PIC1_DATA);
    ARCH::io_wait();
    ARCH::outb(a2, PIC2_DATA);
}

#pragma GCC diagnostic pop
