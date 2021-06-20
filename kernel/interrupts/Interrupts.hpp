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

struct InterruptFrame_t{
    uword_t rbx;
    uword_t rcx;
    uword_t rdx;
    uword_t rsi;
    uword_t rdi;
    uword_t r8;
    uword_t r9;
    uword_t r10;
    uword_t r11;
    uword_t r12;
    uword_t r13;
    uword_t r14;
    uword_t r15;
    uword_t rax;
    const uword_t StackRBP;
    uword_t error_code;
    uword_t ip;
    uword_t cs;
    uword_t flags;
    uword_t sp;
    uword_t ss;
} __attribute__((packed));

__BEGIN_DECLS

/*                                  Vector nr.  Error code?     */
void PitINT()                       /* 0x20         NO          */;
void SIMDINT()                      /* 0x13         NO          */;
void DebugINT()                     /* 0x01         NO          */;
void OverflowINT()                  /* 0x04         NO          */;
void EmptyIqrINT()                  /* ANY          NO          */;
void PageFaultINT()                 /* 0x0E         YES         */;
void InavlidTSSINT()                /* 0x0A         YES         */;
void BreakpointINT()                /* 0x03         NO          */;
void BoundRangeINT()                /* 0x05         NO          */;
void DoubleFaultINT()               /* 0x08         YES (zero)  */;
void MachineCheckINT()              /* 0x12         NO          */;
void StackSegmentINT()              /* 0x0C         YES         */;
void InvalidOpcodeINT()             /* 0x06         NO          */;
void VirtualizationINT()            /* 0x14         NO          */;
void AlignmentCheckINT()            /* 0x11         YES         */;
void DevisionByZeroINT()            /* 0x00         NO          */;
void SegmentNotPresentINT()         /* 0x0B         YES         */;
void GeneralProtectionINT()         /* 0x0D         YES         */;
void DeviceNotAvailableINT()        /* 0x07         NO          */;

void PitINT_Handler                 (InterruptFrame_t *frame);
void SIMDINT_Handler                (InterruptFrame_t *frame);
void DebugINT_Handler               (InterruptFrame_t *frame);
void OverflowINT_Handler            (InterruptFrame_t *frame);
void EmptyIqrINT_Handler            (InterruptFrame_t *frame);
void BreakpointINT_Handler          (InterruptFrame_t *frame);
void BoundRangeINT_Handler          (InterruptFrame_t *frame);
void MachineCheckINT_Handler        (InterruptFrame_t *frame);
void InvalidOpcodeINT_Handler       (InterruptFrame_t *frame);
void VirtualizationINT_Handler      (InterruptFrame_t *frame);
void DevisionByZeroINT_Handler      (InterruptFrame_t *frame);
void DeviceNotAvailableINT_Handler  (InterruptFrame_t *frame);

void PageFaultINT_Handler           (InterruptFrame_t *frame, int error_code);
void InavlidTSSINT_Handler          (InterruptFrame_t *frame, int error_code);
void DoubleFaultINT_Handler         (InterruptFrame_t *frame, int error_code);
void StackSegmentINT_Handler        (InterruptFrame_t *frame, int error_code);
void AlignmentCheckINT_Handler      (InterruptFrame_t *frame, int error_code);
void GeneralProtectionINT_Handler   (InterruptFrame_t *frame, int error_code);
void SegmentNotPresentINT_Handler   (InterruptFrame_t *frame, int error_code);

__END_DECLS

void RemapPIC();

always_inline void PIC_EndMaster(){
    ARCH::outb(PIC_EOI, PIC1_COMMAND);
}

always_inline void PIC_EndSlave(){
    ARCH::outb(PIC_EOI, PIC2_COMMAND);
    ARCH::outb(PIC_EOI, PIC1_COMMAND);
}

#endif /* Interrupts_hpp */

