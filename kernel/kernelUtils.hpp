#ifndef kernelUtils_hpp
#define kernelUtils_hpp

#include <kernel.h>
#include <gdt/gdt.hpp>
#include <printf/Printf.h>
#include <interrupts/IDT.hpp>
#include <interrupts/Interrupts.h>
#include <memory/VMManager.hpp>
#include <basicFrameManager.hpp>

class KernelInfo{
    static IDTR idtr;
    static PageTable *PML4;
    static GDTDescriptor gdt;

public:
    static VMManager KernelVMM;

private:
    static void InitVMM();
    static void InitGDT();
    static void InitIDT();

public:
    static inline const PageTable *GetPageTableAddr() { return PML4; }

public:
    static void Init();

};

#endif /* kernelUtils_hpp */
