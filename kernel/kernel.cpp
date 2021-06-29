#include <cerberus/io.h>
#include "kernelUtils.hpp"
#include "memory/VMManager.hpp"
#include "render/basicFrameManager.hpp"
#include "memory/kmalloc.h"
#include <cerberus/printf.h>

kernel_services_t *KS;

extern "C" int _start(kernel_services_t *services){
    KS = services;
    CPutchar = BasicRender::PutChar;
    CSetColor = BasicRender::SetColor;
   
    BasicRender::SetFrameBuffer(&KS->frameBuffer);
    KernelInfo::Init();

    cerb::Go2Sleep();
    return 0;
}
