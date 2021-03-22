#include <Interrupts.h>
#include <printf/Printf.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

extern void devider();

__attribute__((interrupt))
void DevideByZero_Handler(struct interrupt_frame *frame){
    u64 rip;

    __asm__ __volatile__(
        "addq $3, 0(%%rax)\n"
        "movq 0(%%rax), %0\n"
        : "=r" (rip)
    );

    Printf("Zero division error at %p\n", rip);

    return;
}

__attribute__((interrupt))
void Debug_Handler(struct interrupt_frame *frame){
    Printf("Debug\n");
    return;
}

__attribute__((interrupt))
void Breakpoint_Handler(struct interrupt_frame *frame){
    Printf("Breakpoint\n");
    return;
}

__attribute__((interrupt))
void Overflow_Handler(struct interrupt_frame *frame){
    Printf("Overflow error\n");
    return;
}

__attribute__((interrupt))
void BoundRange_Handler(struct interrupt_frame *frame){
    Printf("Bound Range error\n");
    return;
}

__attribute__((interrupt))
void InvalidOpcode_Handler(struct interrupt_frame *frame){
    Printf("InvalidOpcode error\n");
    return;
}

__attribute__((interrupt))
void DeviceNotAvailable_Handler(struct interrupt_frame *frame){
    Printf("Device not available error\n");
    return;
}

__attribute__((interrupt))
void DoubleFault_Handler(struct interrupt_frame *frame){
    Printf("Warning double fault accured! Stop execution!\n");

    while (1){
        __asm__ __volatile__(
            "hlt"
        );
    }

    return; 
}

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame *frame){
    Printf("Page fault\n");
    
    return;
}

#pragma clang diagnostic pop
