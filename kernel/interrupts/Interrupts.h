#ifndef Interrupts_hpp
#define Interrupts_hpp

#include <arch.hpp>
#include <kernel.h>

#ifdef __x86_64__
  typedef unsigned long long int uword_t;
#else
  typedef unsigned int uword_t;
#endif

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame
{
    uword_t ip;
    uword_t cs;
    uword_t flags;
    uword_t sp;
    uword_t ss;
};

__attribute__((interrupt))
void Debug_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void Breakpoint_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void Overflow_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void BoundRange_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void InvalidOpcode_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void DeviceNotAvailable_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void DoubleFault_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void DevideByZero_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void SegmentNotPresent_Handler(struct interrupt_frame *frame);

__attribute__((interrupt))
void GeneralProtection_Handler(struct interrupt_frame *frame);

__attribute__((interrupt)) 
void PageFault_Handler(struct interrupt_frame *frame, unsigned long error_code);

__attribute__((interrupt)) 
void Pit_Handler(struct interrupt_frame *frame);

__attribute__((interrupt)) 
void EmptyIQR_Handler(struct interrupt_frame *frame);

void RemapPIC();

always_inline void PIC_EndMaster(){
    ARCH::outb(PIC_EOI, PIC1_COMMAND);
}

always_inline void PIC_EndSlave(){
    ARCH::outb(PIC_EOI, PIC2_COMMAND);
    ARCH::outb(PIC_EOI, PIC1_COMMAND);
}

#endif /* Interrupts_hpp */
