#ifndef IDT_hpp
#define IDT_hpp

#include <kernel.h>

#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

struct IDTDescriptorEntry {
    u16 offset0;
    u16 selector;
    u8  ist;
    u8  type_attr;
    u16 offset1;
    u32 offset2;
    u32 ignore;

public:

    inline void setOffset(u64 offset){
        this->offset0 = (u16)(offset & 0x000000000000ffff);
        this->offset1 = (u16)((offset & 0x00000000ffff0000) >> 16);
        this->offset2 = (u32)((offset & 0xffffffff00000000) >> 32);
    }

    u64 GetOffset(){
         u64 offset = this->offset0;
        
        offset |= (u64) this->offset1 << 16;
        offset |= (u64) this->offset2 << 32;
        
        return offset;
    }
} __attribute__((packed));

struct IDTR{
    u16 limit;
    u64 offset;
} __attribute__((packed));

#endif /* IDT_hpp */
