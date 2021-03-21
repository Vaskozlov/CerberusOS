#include <kernel.h>
#include <printf/Printf.h>
#include <memory.hpp>
#include <basicFrameManager.hpp>
#include <BitMap.hpp>
#include <string.h>

kernel_services_t *KS;
u8 bits[20];

extern "C" int _start(kernel_services_t *services){
    KS = services;
    Putchar = BasicRender::PutChar;

    BasicRender::SetFrameBuffer(&KS->frameBuffer);
    BasicRender::ClearScreen();
    pMemoryManager::init();

    PageTable *PML4 = (PageTable*) pMemoryManager::RequestPage();
    memset(PML4, 0, 0x1000);

    vMemoryManager vManager(PML4);

    Printf("%d\n", pMemoryManager::GetMapEnteries());

    for (size_t i = 0 ; i <= pMemoryManager::GetSystemMemory() + 1; i += 0x1000){
    
        vManager.mapMemory((void*)(i), (void*)(i));
    }

    u64 fbBase = (u64)KS->frameBuffer.base_address;
    u64 fbSize = (u64)KS->frameBuffer.buffer_size + 0x10000;

    for (size_t i = fbBase; i < fbBase + fbSize; i += 0x1000){
        vManager.mapMemory((void*)(i * 0x1000), (void*)(i * 0x1000));
    }

    __asm__ __volatile__(
        "mov %0, %%cr3\n"
        :
        : "r" (PML4)
    );
    

    Printf("\n\n\ntest");

    return 123;
}
