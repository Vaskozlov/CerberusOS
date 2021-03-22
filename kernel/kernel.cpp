#include <kernelUtils.hpp>
#include <VMManager.hpp>
#include <Interrupts.h>

kernel_services_t *KS;


extern "C" int _start(kernel_services_t *services){
    KS = services;
    Putchar = BasicRender::PutChar;

    BasicRender::SetFrameBuffer(&KS->frameBuffer);
    KernelInfo::Init();
    
    
    while (1){
        __asm__ __volatile__ (
            "hlt\n"
        );
    }
    return 0;
}
