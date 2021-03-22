#include <kernel.h>
#include <printf/Printf.h>
#include <VMManager.hpp>
#include <basicFrameManager.hpp>
#include <PhisicalAllocator.hpp>
#include <string.h>
#include <gdt.hpp>
#include <IDT.hpp>
#include <Interrupts.h>

kernel_services_t *KS;

void MapKernelInside(VMManager *manager, void *PML4){
    u64 fbBase = (u64)KS->frameBuffer.base_address;
    u64 fbSize = (u64)KS->frameBuffer.buffer_size + 0x10000;
        
    for (size_t i = fbBase; i < fbBase + fbSize; i += 0x1000){
        manager->MapMemory((void*)i, (void*)i);
    }

    for (size_t i = 0 ; i <= PhisicalAllocator::GetTotalMemory() + 1; i += 0x1000){
        manager->MapMemory((void*)i, (void*)i);
    }

    __asm__ __volatile__(
        "mov %0, %%cr3\n"
        :
        : "r" (PML4)
    );
}


void initIDT(){
    static IDTR idtr;
    idtr.limit = 0x0FFF;
    idtr.offset = (u64) PhisicalAllocator::Get();

    IDTDescriptorEntry *int_PageFault = (IDTDescriptorEntry*)(idtr.offset + 0xE * sizeof(IDTDescriptorEntry));
    int_PageFault->setOffset((u64)PageFault_Handler);
    int_PageFault->type_attr = IDT_TA_InterruptGate;
    int_PageFault->selector = 0x08;

    __asm__ __volatile__ (
        "lidt %0\n"
        :
        : "m" (idtr)
    );
}

extern "C" int _start(kernel_services_t *services){
    KS = services;
    Putchar = BasicRender::PutChar;

    BasicRender::SetFrameBuffer(&KS->frameBuffer);
    BasicRender::ClearScreen();
    PhisicalAllocator::Init();

    PageTable *PML4 = (PageTable*) PhisicalAllocator::Get();
    memset(PML4, 0, 0x1000);

    VMManager vManager(PML4);
    MapKernelInside(&vManager, PML4);
    
    BasicRender::ClearScreen();
    Printf("Virtual manager ready %d\n", PhisicalAllocator::GetAvailableMemory() / 1024);

    static GDTDescriptor gdt;
    gdt.size = sizeof(GDT) - 1;
    gdt.address = (u64)&DefaultGDT;
    
    LoadGDT(&gdt);
    Printf("GDT ready\n");

    initIDT();
    Printf("IDT ready\n");

    __asm__ __volatile__ (
        "int $0x0e"
    );

    Printf("Test\n");

    while (1){
        __asm__ __volatile__ (
            "hlt\n"
        );
    }
    return 0;
}
