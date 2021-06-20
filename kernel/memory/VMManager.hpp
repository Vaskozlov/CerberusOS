#ifndef VMManager_hpp
#define VMManager_hpp

#include <kernel.h>
#include "PhisicalAllocator.hpp"

enum PageDirectoryFlags{
    NotPresented    = 0b0,
    present         = 0b1,
    readWrite       = 0b10,
    superuser       = 0b100,
    writeThrough    = 0b1000,
    cacheDisabled   = 0b10000,
    accessed        = 0b100000,
    ignore0         = 0b1000000,
    largePage       = 0b10000000,
    ignore1         = 0b100000000,
    available0      = 0b1000000000,
    available1      = 0b10000000000,
    available2      = 0b100000000000,
    PageSize4KB     = 0b1000000000000,
    PageSize2MB     = 0b10000000000000,
    PageSize1GB     = 0b100000000000000,
};


class PageDirectoryEntry{
    u64 value;

public:
    always_inline void SetFlag(PageDirectoryFlags flag)        { value |= flag;    }
    always_inline void RempoveFlag(PageDirectoryFlags flag)    { value &= ~flag;   }

    always_inline void SetAddress(void *address) {
        u64 naddress = (u64)address - (u64)address % 0x1000;
        value |= naddress;
    }

    always_inline void *GetAddress(){
        return (void*)(value - (value % 0x1000));
    }

    always_inline PageDirectoryFlags GetFlags(){
        return (PageDirectoryFlags)(value % 0x1000);
    }

    always_inline bool IsFlagSet(PageDirectoryFlags flag){
        return (value & flag) > 0;
    }

    always_inline void RemoveFlags(){
        value -= value % 0x1000;
    }

public:
    PageDirectoryEntry() = default;
};

struct PageMapIndexer4KBMPL4{
    u64 PDP_i;
    u64 PD_i;
    u64 PT_i;
    u64 P_i;

    inline PageMapIndexer4KBMPL4(u64 VirtualAddress){
        VirtualAddress >>= 12;

        P_i = VirtualAddress & 0x1FF;
        VirtualAddress >>= 9;

        PT_i = VirtualAddress & 0x1FF;
        VirtualAddress >>= 9;

        PD_i = VirtualAddress & 0x1FF;
        VirtualAddress >>= 9;

        PDP_i = VirtualAddress & 0x1FF;
    }
};

struct PageMapIndexer2MBPML4{
    u64 PDP_i;
    u64 PD_i;
    u64 PT_i;

    inline PageMapIndexer2MBPML4(u64 VirtualAddress){
        VirtualAddress >>= 21;

        PT_i = VirtualAddress & 0x1FF;
        VirtualAddress >>= 9;

        PD_i = VirtualAddress & 0x1FF;
        VirtualAddress >>= 9;

        PDP_i = VirtualAddress & 0x1FF;
    }
};

struct PageMapIndexer1GBPML4{
    u64 PDP_i;
    u64 PD_i;

    inline PageMapIndexer1GBPML4(u64 VirtualAddress){
        VirtualAddress >>= 30;

        PD_i = VirtualAddress & 0x1FF;
        VirtualAddress >>= 9;

        PDP_i = VirtualAddress & 0x1FF;
    }
};

struct PageTable{
    PageDirectoryEntry entries[512];
} __attribute__((aligned(0x1000)));

class VMManager{
    PageTable *PML4;
    size_t MappedPages4KB;
    size_t MappedPages2MB;
    size_t MappedPages1GB;

public:
    always_inline size_t GetMappedPages4KB() { return MappedPages4KB; }
    always_inline size_t GetMappedPages2MB() { return MappedPages2MB; }
    always_inline size_t GetMappedPages1GB() { return MappedPages1GB; }

public:
    PageDirectoryFlags GetPageFlags(void *virtualMemory);
    void MapMemory4KB(void *virtualMemory, void *PhysicalAddress);
    void MapMemory2MB(void *virtualMemory, void *PhysicalAddress);
    void MapMemory1GB(void *virtualMemory, void *PhysicalAddress);

    void UnMapMemory4KB(void *virtualMemory, void *PhysicalAddress);
    void UnMapMemory2MB(void *virtualMemory, void *PhysicalAddress);
    void UnMapMemory1GB(void *virtualMemory, void *PhysicalAddress);

    inline VMManager(PageTable *PML4Address) : PML4(PML4Address), MappedPages4KB(0) {}
    VMManager() = default;
};

void PrintPDE(PageDirectoryFlags flags);

#endif /* VMManager_hpp */
