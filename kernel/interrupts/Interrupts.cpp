#include "Interrupts.h"
#include <printf/Printf.h>
#include "scheduling/pit/pit.hpp"
#include <memory/VMManager.hpp>
#include <memory/kmalloc.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

extern VMManager KernelVMM;

__attribute__((interrupt))
void DevideByZero_Handler(struct interrupt_frame *frame){
   
    kprintf("Zero division error\n");
    ARCH::Go2Sleep();

    return;
}

__attribute__((interrupt))
void Debug_Handler(struct interrupt_frame *frame){

    kprintf("Debug\n");
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void Breakpoint_Handler(struct interrupt_frame *frame){
    
    kprintf("Breakpoint\n");
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void Overflow_Handler(struct interrupt_frame *frame){
    
    kprintf("Overflow error\n");
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void BoundRange_Handler(struct interrupt_frame *frame){
    
    kprintf("Bound Range error\n");
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void InvalidOpcode_Handler(struct interrupt_frame *frame){
    
    kprintf("InvalidOpcode error\n");
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void DeviceNotAvailable_Handler(struct interrupt_frame *frame){
    
    kprintf("Device not available error\n");
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void DoubleFault_Handler(struct interrupt_frame *frame){
    
    kprintf("Warning double fault accured! Stop execution!\n");
    ARCH::Go2Sleep();
 
}

__attribute__((interrupt)) 
void GeneralProtection_Handler(struct interrupt_frame *frame){

    kprintf("GP fault at %p\n", frame->ip);
    ARCH::Go2Sleep();
}

__attribute__((interrupt))
void SegmentNotPresent_Handler(struct interrupt_frame *frame){
    
    kprintf("Segment not present error\n");
    ARCH::Go2Sleep();
}

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame *frame, unsigned long error_code){
    u64 memoryRegion;

    __asm__ __volatile__(
        "movq %%cr2, %0\n"
        : "=r" (memoryRegion)
    );
   
    if (memoryRegion != 0x0 && memoryRegion >= MallocMainHeder.MallocBegin && memoryRegion < MallocMainHeder.MallocHead){
        auto page = PhisicalAllocator::Get2MB();
        KernelVMM.MapMemory2MB((void*)(memoryRegion - (memoryRegion & ((1<<21UL) - 1))), page);
    }
    else{
        kprintf("PANIC!!! Page fault with error %u. Target address was %p. At line %p\n", error_code, memoryRegion, frame->ip);
        ARCH::Go2Sleep();
    }

    return;
}

__attribute__((interrupt)) 
void EmptyIQR_Handler(struct interrupt_frame *frame){
    PIC_EndMaster();
    return;
}

__attribute__((interrupt)) 
void Pit_Handler(struct interrupt_frame *frame){
    PIT::Tick();
    PIC_EndMaster();
}
   
void RemapPIC(){
    uint8_t a1, a2; 

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
