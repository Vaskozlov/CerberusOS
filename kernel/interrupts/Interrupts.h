#ifndef Interrupts_hpp
#define Interrupts_hpp

struct interrupt_frame;
__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame *frame);

#endif /* Interrupts_hpp */
