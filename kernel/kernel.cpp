#include "kernelUtils.hpp"
#include "memory/VMManager.hpp"
#include "printf/Printf.h"
#include "render/basicFrameManager.hpp"
#include "memory/kmalloc.h"
#include <arch.hpp>

kernel_services_t *KS;

char *message = "Hello world\0\0\0\0";
char buffer[32];

extern "C" int _start(kernel_services_t *services){
    KS = services;
    PutcharWay[0] = BasicRender::PutChar;
    SetColorWay[0] = BasicRender::SetColor;

    BasicRender::SetFrameBuffer(&KS->frameBuffer);
    KernelInfo::Init();

    memcpy64(buffer, message, 3);
    kprintf(buffer);

    ARCH::Go2Sleep();
    return 0;
}
