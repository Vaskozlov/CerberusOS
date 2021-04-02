#include "kernelUtils.hpp"
#include "memory/VMManager.hpp"
#include "printf/Printf.h"
#include "render/basicFrameManager.hpp"
#include "memory/kmalloc.h"

kernel_services_t *KS;

extern "C" int _start(kernel_services_t *services){
    KS = services;
    PutcharWay[0] = BasicRender::PutChar;
    SetColorWay[0] = BasicRender::SetColor;

    BasicRender::SetFrameBuffer(&KS->frameBuffer);
    
    KernelInfo::Init();
    while (1){ __asm__ __volatile__("hlt"); }
    
    return 0;
}
