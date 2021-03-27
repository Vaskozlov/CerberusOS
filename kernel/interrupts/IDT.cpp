#include "IDT.hpp"

void IDTDescriptorEntry::setOffset(u64 offset){
    this->offset0 = (u16)(offset & 0x000000000000ffff);
    this->offset1 = (u16)((offset & 0x00000000ffff0000) >> 16);
    this->offset2 = (u32)((offset & 0xffffffff00000000) >> 32);
}

u64 IDTDescriptorEntry::GetOffset(){
    u64 offset = this->offset0;
    offset |= (u64) this->offset1 << 16;
    offset |= (u64) this->offset2 << 32;

    return offset;
}
