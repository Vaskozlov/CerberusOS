#ifndef gdt_h
#define gdt_h

#include <kernel.h>

struct GDTDescriptor{
    u16     size;
    u64     address;
} __attribute__((packed));

struct GDTEntry{
    u16     limit0;
    u16     base0;
    u8      base1;
    u8      accessByte;
    u8      limit1Flag;
    u8      Base2;
} __attribute__((packed));

struct GDT{
    GDTEntry Null;
    GDTEntry KernelCode;
    GDTEntry KernelData;
    GDTEntry UserNull;
    GDTEntry UserCode;
    GDTEntry UserDara;
} __attribute__((packed))
__attribute__((aligned(0x10)));

extern GDT DefaultGDT;
extern "C" void LoadGDT(GDTDescriptor *gdtDescriptor);

#endif /* gdt_h */
