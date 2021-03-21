#ifndef memory_h
#define memory_h

#include <kernel.h>
#include <BitMap.hpp>

struct PageDirectoryEntry{
    bool present        : 1;
    bool readWrite      : 1;
    bool superUser      : 1;
    bool writeThrough   : 1;
    bool cacheDisabled  : 1;
    bool Accessed       : 1;
    bool ignore0        : 1;
    bool largePages     : 1;
    bool ignore1        : 1;
    u8 available        : 3;
    u64 address         : 52;
};

struct PageTable{
    PageDirectoryEntry entries[512];
} __attribute__((aligned(0x1000)));

class pMemoryManager{

    static u64 SystemMemory;
    static u64 AvailableMemory;
    static u64 ReservedMemory;
    static u64 UsedMemory;
    static u64 mMapEnteries;
    static u64 *KernelStart;
    static u64 *KernelEnd;
    static BitMap PageBitMap;

public:
    static const char *EFI_MEMORY_TYPE_STRING[];

public:
    inline static u64 GetAvailableMemory()  {   return AvailableMemory;   }
    inline static u64 GetReservedMemory()   {   return ReservedMemory;    }
    inline static u64 GetUsedMemory()       {   return UsedMemory;        }
    inline static u64 GetMapEnteries()      {   return mMapEnteries;      }
    inline static u64 GetSystemMemory()     {   return SystemMemory;      }
    inline static u64 *GetKernelStart()     {   return KernelStart;       }
    inline static u64 *GetKernelEnd()       {   return KernelEnd;         }

private:
    static void FreePage(void *address);
    static void FreePages(void *address, size_t amount);

    static void LockPage(void *address);
    static void LockPages(void *address, size_t amount);

public:
    static void Reserve(void *address);
    static void UnReserve(void *address);
    static void Reserve(void *address, size_t amount);
    static void UnReserve(void *address, size_t amount);

public:
    static void *RequestPage();

public:
    static void init();

public:
    pMemoryManager() = delete;
    ~pMemoryManager() = delete;
};

struct PageMapIndexer{
    u64 PDP_i;
    u64 PD_i;
    u64 PT_i;
    u64 P_i;

    PageMapIndexer(u64 VirtualAddress);

};

class vMemoryManager{
    PageTable *PML4;

public:
    void mapMemory(void *virualAddress, void *PhysicalAddress);

public:
    inline vMemoryManager(PageTable *PML4Address) : PML4(PML4Address) {}
};

#endif /* memory_h */
