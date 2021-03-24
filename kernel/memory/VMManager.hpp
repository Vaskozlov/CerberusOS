#ifndef VMManager_hpp
#define VMManager_hpp

#include <kernel.h>
#include <printf/Printf.h>
#include <PhisicalAllocator.hpp>

enum PageDirectoryFlags{
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
    available2      = 0b100000000000 
};

class PageDirectoryEntry{
    u64 value;

public:
    inline void SetFlag(PageDirectoryFlags flag)        { value |= flag;    }
    inline void RempoveFlag(PageDirectoryFlags flag)    { value &= ~flag;   }

    inline void SetAddress(void *address) {
        u64 naddress = (u64)address - (u64)address % 0x1000;
        value |= naddress;
    }

    inline void *GetAddress(){
        return (void*)(value - (value % 0x1000));
    }

    inline bool IsFlagSet(PageDirectoryFlags flag){
        return (value & flag) > 0;
    }

public:
    PageDirectoryEntry() = default;
};

struct PageMapIndexer{
    u64 PDP_i;
    u64 PD_i;
    u64 PT_i;
    u64 P_i;

    PageMapIndexer(u64 VirtualAddress);
};

struct PageMapIndexerLVL3{
    u64 PDP_i;
    u64 PD_i;
    u64 PT_i;
    u64 P_i;

    inline PageMapIndexerLVL3(u64 VirtualAddress){
        VirtualAddress >>= 21;

        PT_i = VirtualAddress & 0x1FF;
        VirtualAddress >>= 9;

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
    size_t MappedPages;

public:
    void MapMemory(void *virtualMemory, void *PhysicalAddress);
    
    inline size_t GetMappedPages() { return MappedPages; }

    inline void *AllocateAndMap(){
        void *page = PhisicalAllocator::Get();
        MapMemory(page, page);
        return page;
    }

    inline void *AllocateAndMap(void *virtualAddress){
        void *page = PhisicalAllocator::Get();
        MapMemory(virtualAddress, page);
        return virtualAddress;
    }

    inline VMManager(PageTable *PML4Address) : PML4(PML4Address), MappedPages(0) {}
    VMManager() = default;
};

#endif /* VMManager_hpp */
