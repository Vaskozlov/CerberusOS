#include "kernelUtils.hpp"
#include "memory/VMManager.hpp"
#include "printf/Printf.h"
#include "render/basicFrameManager.hpp"


kernel_services_t *KS;

extern "C" int _start(kernel_services_t *services){
    KS = services;
    Putchar = BasicRender::PutChar;

    BasicRender::SetFrameBuffer(&KS->frameBuffer);
    BasicRender::FontColor.value = COLOR_GREEN;

    KernelInfo::Init();
   
    while (1){ __asm__ __volatile__("hlt"); }
    
    return 0;
}
