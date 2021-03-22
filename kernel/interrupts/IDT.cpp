#include <IDT.hpp>

void IDTDescriptorEntry::setOffset(u64 offset){
    this->offset0 = (u16)(offset & 0x000000000000FFFF);
    offset >>= 16;
    this->offset1 = (u16)(offset & 0x00000000FFFF0000);
    offset >>= 16;
    this->offset2 = (u32)(offset & 0xFFFFFFFF00000000);
}

u64 IDTDescriptorEntry::GetOffset(){
    u64 offset = this->offset0;
    offset |= (u64) this->offset1 << 16;
    offset |= (u64) this->offset2 << 32;

    return offset;
}
