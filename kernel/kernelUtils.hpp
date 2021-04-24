#ifndef kernelUtils_hpp
#define kernelUtils_hpp

#include "kernel.h"
#include "gdt/gdt.hpp"
#include "interrupts/IDT.hpp"
#include "memory/VMManager.hpp"

extern VMManager KernelVMM;

class KernelInfo{
    static IDTR idtr;
    static PageTable *PML4;
    static GDTDescriptor gdt;

private:
    static void SetUpIDTEntry(void *handler, u16 position, u8 selector, u8 type_attr);

private:
    static void InitVMM();
    static void InitGDT();
    static void InitIDT();
    static void InitACPI();

public:
    static strict_inline const PageTable *GetPageTableAddr() { return PML4; }

public:
    static void Init();
};

#endif /* kernelUtils_hpp */
