#include "Interrupts.h"
#include <hardware/io.hpp>
#include <printf/Printf.h>
#include "scheduling/pit/pit.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

inline void Go2Sleep() { while (1) {__asm__ __volatile__ ("hlt");} }

__attribute__((interrupt))
void DevideByZero_Handler(struct interrupt_frame *frame){
   
    Printf("Zero division error\n");
    
    Go2Sleep();
    return;
}

__attribute__((interrupt))
void Debug_Handler(struct interrupt_frame *frame){
    Printf("Debug\n");

    Go2Sleep();
    return;
}

__attribute__((interrupt))
void Breakpoint_Handler(struct interrupt_frame *frame){
    Printf("Breakpoint\n");

    Go2Sleep();
    return;
}

__attribute__((interrupt))
void Overflow_Handler(struct interrupt_frame *frame){
    Printf("Overflow error\n");

    Go2Sleep();
    return;
}

__attribute__((interrupt))
void BoundRange_Handler(struct interrupt_frame *frame){
    Printf("Bound Range error\n");

    Go2Sleep();
    return;
}

__attribute__((interrupt))
void InvalidOpcode_Handler(struct interrupt_frame *frame){
    Printf("InvalidOpcode error\n");

    Go2Sleep();
    return;
}

__attribute__((interrupt))
void DeviceNotAvailable_Handler(struct interrupt_frame *frame){
    Printf("Device not available error\n");

    Go2Sleep();
    return;
}

__attribute__((interrupt))
void DoubleFault_Handler(struct interrupt_frame *frame){
    Printf("Warning double fault accured! Stop execution!\n");

    Go2Sleep();
    return; 
}

__attribute__((interrupt)) 
void GeneralProtection_Handler(struct interrupt_frame *frame){
    Printf("GP fault at %p\n", frame->ip);

    Go2Sleep();
    return;
}

__attribute__((interrupt))
void SegmentNotPresent_Handler(struct interrupt_frame *frame){
    Printf("Segment not present error\n");

    Go2Sleep();
    return;
}

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame *frame, unsigned long error_code){
    u64 memoryRegion; 

    __asm__ __volatile__(
        "movq %%cr2, %0\n"
        : "=r" (memoryRegion)
    );
   
    Printf("Page fault with error %u. Target address was %p. At line %p\n", error_code, memoryRegion, frame->ip);
    
    Go2Sleep();
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

void PIC_EndMaster(){
    outb(PIC_EOI, PIC1_COMMAND);
}

void PIC_EndSlave(){
    outb(PIC_EOI, PIC2_COMMAND);
    outb(PIC_EOI, PIC1_COMMAND);
}
   
void RemapPIC(){
    uint8_t a1, a2; 

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND);
    io_wait();
    outb(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND);
    io_wait();

    outb(0x20, PIC1_DATA);
    io_wait();
    outb(0x28, PIC2_DATA);
    io_wait();

    outb(4, PIC1_DATA);
    io_wait();
    outb(2, PIC2_DATA);
    io_wait();

    outb(ICW4_8086, PIC1_DATA);
    io_wait();
    outb(ICW4_8086, PIC2_DATA);
    io_wait();

    outb(a1, PIC1_DATA);
    io_wait();
    outb(a2, PIC2_DATA);
}

#pragma clang diagnostic pop
