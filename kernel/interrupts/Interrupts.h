#ifndef Interrupts_hpp
#define Interrupts_hpp

struct interrupt_frame;

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
void PageFault_Handler(struct interrupt_frame *frame);

#endif /* Interrupts_hpp */
