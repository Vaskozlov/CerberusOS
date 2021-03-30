#include "kernelUtils.hpp"
#include "memory/VMManager.hpp"
#include "printf/Printf.h"
#include "render/basicFrameManager.hpp"
#include "memory/kmalloc.h"

kernel_services_t *KS;

#if defined(CERBERUS_VERSION)
#define CUR_VER CERBERUS_VERSION
#else
#define CUR_VER 0
#endif

extern "C" int _start(kernel_services_t *services){
    KS = services;
    Putchar = BasicRender::PutChar;

    BasicRender::SetFrameBuffer(&KS->frameBuffer);

    KernelInfo::Init();
    
    auto p1 = kmalloc(0x1000);
    auto p2 = kmalloc(0x1000 * 10);
    auto p3 = kmalloc((1<<21UL));
    auto p4 = kmalloc(0x1000);
    
    kfree(p4);
    kfree(p2);
    kfree(p3);
    kfree(p1);

    auto p5 = kmalloc(3<<21UL);
    
    Printf("free done\n");
    
    kMallocElem_t *elem = MallocMainHeder.firstElem;

    while (elem != NULL){

        Printf("cur: %p next: %p prev: %p %x %u\n", elem, elem->next, elem->previous, elem->size, elem->free);

        elem = elem->next;
    }

    while (1){ __asm__ __volatile__("hlt"); }
    
    return 0;
}
