#include <Interrupts.h>
#include <printf/Printf.h>

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame *frame){
    Printf("Page fault\n");
    
    return;
}
