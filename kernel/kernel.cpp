#include "kernelUtils.hpp"
#include "memory/VMManager.hpp"
#include "printf/Printf.h"
#include "render/basicFrameManager.hpp"
#include "memory/kmalloc.hpp"

kernel_services_t *KS;

extern "C" int _start(kernel_services_t *services){
    KS = services;
    Putchar = BasicRender::PutChar;

    BasicRender::SetFrameBuffer(&KS->frameBuffer);

    KernelInfo::Init();
   
    InitKMalloc();

    Printf("Allocating 0x1000000 bytes %p\n", kmalloc(0x1000000));
    Printf("Allocating 0x1000 bytes %p\n", kmalloc(0x1000));
   
    while (1){ __asm__ __volatile__("hlt"); }
    
    return 0;
}
