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

struct interrupt_frame{
    uword_t ip;
    uword_t cs;
    uword_t flags;
    uword_t sp;
    uword_t ss;
};

__attribute__((interrupt))
void DevideByZero_Handler_SSE       /* 0x00 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void Debug_Handler_SSE              /* 0x01 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void Breakpoint_Handler_SSE         /* 0x03 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void Overflow_Handler_SSE           /* 0x05 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void BoundRange_Handler_SSE         /* 0x04 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void InvalidOpcode_Handler_SSE      /* 0x06 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void DeviceNotAvailable_Handler_SSE /* 0x07 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void DoubleFault_Handler_SSE        /* 0x08 */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt))
void InvalidTSS_Handler_SSE         /* 0x0A */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void SegmentNotPresent_Handler_SSE  /* 0x0B */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void StackSegment_Handler_SSE       /* 0x0C */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt))
void GeneralProtection_Handler_SSE  /* 0x0D */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt)) 
void PageFault_Handler_SSE          /* 0x0E */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt))
void AlignmentCheck_Handler_SSE     /* 0x11 */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt))
void MachineCheck_Handler_SSE       /* 0x12 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void SIMD_Handler_SSE               /* 0x13 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void Virtualization_Handler_SSE     /* 0x14 */  (struct interrupt_frame *frame);

__attribute__((interrupt)) 
void Pit_Handler_SSE                /* 0x20 */  (struct interrupt_frame *frame);

__attribute__((interrupt)) 
void EmptyIQR_Handler_SSE           /* ANY IRQ */ (struct interrupt_frame *frame);

__attribute__((interrupt))
void DevideByZero_Handler_AVX       /* 0x00 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void Debug_Handler_AVX              /* 0x01 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void Breakpoint_Handler_AVX         /* 0x03 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void Overflow_Handler_AVX           /* 0x05 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void BoundRange_Handler_AVX         /* 0x04 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void InvalidOpcode_Handler_AVX      /* 0x06 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void DeviceNotAvailable_Handler_AVX /* 0x07 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void DoubleFault_Handler_AVX        /* 0x08 */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt))
void InvalidTSS_Handler_AVX         /* 0x0A */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void SegmentNotPresent_Handler_AVX  /* 0x0B */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void StackSegment_Handler_AVX       /* 0x0C */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt))
void GeneralProtection_Handler_AVX  /* 0x0D */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt)) 
void PageFault_Handler_AVX          /* 0x0E */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt))
void AlignmentCheck_Handler_AVX     /* 0x11 */  (struct interrupt_frame *frame, uword_t error_code);

__attribute__((interrupt))
void MachineCheck_Handler_AVX       /* 0x12 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void SIMD_Handler_AVX               /* 0x13 */  (struct interrupt_frame *frame);

__attribute__((interrupt))
void Virtualization_Handler_AVX     /* 0x14 */  (struct interrupt_frame *frame);

__attribute__((interrupt)) 
void Pit_Handler_AVX                /* 0x20 */  (struct interrupt_frame *frame);

__attribute__((interrupt)) 
void EmptyIQR_Handler_AVX           /* ANY IRQ */ (struct interrupt_frame *frame);

void RemapPIC();

always_inline void PIC_EndMaster(){
    ARCH::outb(PIC_EOI, PIC1_COMMAND);
}

always_inline void PIC_EndSlave(){
    ARCH::outb(PIC_EOI, PIC2_COMMAND);
    ARCH::outb(PIC_EOI, PIC1_COMMAND);
}

#endif /* Interrupts_hpp */
