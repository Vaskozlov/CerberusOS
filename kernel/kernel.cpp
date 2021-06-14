#include "kernelUtils.hpp"
#include "memory/VMManager.hpp"
#include "printf/Printf.h"
#include "render/basicFrameManager.hpp"
#include "memory/kmalloc.h"
#include <arch.hpp>
#include <cerberus/printf.h>

kernel_services_t *KS;

extern "C" int _start(kernel_services_t *services){
    KS = services;
    PutcharWay[0] = BasicRender::PutChar;
    SetColorWay[0] = BasicRender::SetColor;
    CPutchar = BasicRender::PutChar;
    CSetColor = BasicRender::SetColor;
   
    BasicRender::SetFrameBuffer(&KS->frameBuffer);
    KernelInfo::Init();

    ARCH::Go2Sleep();
    return 0;
}
