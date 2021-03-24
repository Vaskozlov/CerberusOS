#ifndef IDT_hpp
#define IDT_hpp

#include <kernel.h>

#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

struct IDTDescriptorEntry {
    u16 offset0;
    u16 selector;
    u8 ist;
    u8 type_attr;
    u16 offset1;
    u32 offset2;
    u32 ignore;

public:
    void setOffset(u64 offset);
    u64 GetOffset();
} __attribute__((packed));

struct IDTR{
    u16 limit;
    u64 offset;
} __attribute__((packed));

#endif /* IDT_hpp */
