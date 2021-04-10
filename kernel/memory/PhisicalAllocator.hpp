#ifndef PhisicalAllocator_hpp
#define PhisicalAllocator_hpp

#include <kernel.h>
#include <structures/BitMap.hpp>

enum EFI_MEMORY_TYPE{
    EfiReservedMemory       = 0,
    EfiLoaderCode           = 1,
    EfiLoaderData           = 2,
    EfiBootServicesCode     = 3,
    EfiBootServicesData     = 4,
    EfiRuntimeServicesCode  = 5,
    EfiRuntimeServicesData  = 6,
    EfiConventionalMemory   = 7,
    EfiUnusableMemory       = 8,
    EfiACPIReclaimMemory    = 9,
    EfiACPIMemoryNVS        = 10,
    EfiMemoryMappedIO       = 11,
    EfiMemoryMappedIOSpace  = 12,
    EfiPalCode              = 13,
    EfiUknownMemory         = 14
};

class PhisicalAllocator{

    static u64  LockedMemory;
    static u64  AllocatorHead;
    static u64  TotalMemory;
    static u64  ReservedMemory;
    static u64  AvailableMemory;
    static u64  mMapEnteries;

    static u64  KernelStart;
    static u64  KernelEnd;

    static BitMapDouble<u64> BigEnteries;
    static BitMapDoubleConst<u64, 512> *MiddleEntries;
    static BitMapConst<u64, 512> *SmallEntries;

    static const char   *EFI_MEMORY_TYPE_STRING[];
    
public:
    static strict_inline u64 GetTotalMemory()     { return TotalMemory;     }
    static strict_inline u64 GetLockedMemory()    { return LockedMemory;    }
    static strict_inline u64 GetReservedMemory()  { return ReservedMemory;  }
    static strict_inline u64 GetAvailableMemory() { return AvailableMemory; }
    static strict_inline u64 GetKernelStart()     { return KernelStart;     }
    static strict_inline u64 GetKernelEnd()       { return KernelEnd;       }

    strict_inline static const char *GetEfiTypeString(EFI_MEMORY_TYPE type) {  
        return type < EfiUknownMemory ? EFI_MEMORY_TYPE_STRING[type] : EFI_MEMORY_TYPE_STRING[EfiUknownMemory];
    }

private:
    static size_t UniversalLock4KB(void* address);
    static size_t UniversalLock2MB(void* address);
    static size_t UniversalLock1GB(void *address);

private:
    static size_t UniversalUnLock4KB(void* address);
    static size_t UniversalUnLock2MB(void* address);
    static size_t UniversalUnLock1GB(void *address);

private:
    static strict_inline void Reserve4KB(void* address){
        auto size = UniversalLock4KB(address);
        AvailableMemory -= size;
        ReservedMemory += size;
    }

    static strict_inline void Reserve2MB(void* address){
        auto size = UniversalLock2MB(address);
        AvailableMemory -= size;
        ReservedMemory += size;
    }
    
    static strict_inline void Reserve1GB(void *address){
        auto size = UniversalLock1GB(address);
        AvailableMemory -= size;
        ReservedMemory += size;
    }

private:
    static strict_inline void UnReserve4KB(void* address){
        auto size = UniversalUnLock4KB(address);
        ReservedMemory -= size;
        AvailableMemory += size;
    }

    static strict_inline void UnReserve2MB(void* address){
        auto size = UniversalUnLock2MB(address);
        ReservedMemory -= size;
        AvailableMemory += size;
    }

    static strict_inline void UnReserve1GB(void *address){
        auto size = UniversalUnLock1GB(address);
        ReservedMemory -= size;
        AvailableMemory += size;
    }

public:
    static strict_inline void Lock4KB(void* address){
        auto size = UniversalLock4KB(address);
        AvailableMemory -= size;
        LockedMemory += size;
    }

    static strict_inline void Lock4KB(void* address, size_t times){
        for (size_t i = 0; i < times; i++){
            auto size = UniversalLock4KB((void*)((u64)address + i * 0x1000));
            AvailableMemory -= size;
            LockedMemory += size;
        }
    }

    static strict_inline void Lock2MB(void* address){
        auto size = UniversalLock2MB(address);
        AvailableMemory -= size;
        LockedMemory += size;
    }

    static strict_inline void Lock2MB(void* address, size_t times){
        for (size_t i = 0; i < times; i++){
            auto size = UniversalLock2MB((void*)((u64)address + (i << 21UL)));
            AvailableMemory -= size;
            LockedMemory += size;
        }
    }
    
    static strict_inline void Lock1GB(void *address){
        auto size = UniversalLock1GB(address);
        AvailableMemory -= size;
        LockedMemory += size;
    }

public:
    static strict_inline void UnLock4KB(void* address){
        auto size = UniversalUnLock4KB(address);
        LockedMemory -= size;
        AvailableMemory += size;
    }

    static strict_inline void UnLock4KB(void* address, size_t times){
        for (size_t i = 0; i < times; i++){
            auto size = UniversalUnLock4KB((void*)((u64)address + i * 0x1000));
            AvailableMemory -= size;
            LockedMemory += size;
        }
    }

    static strict_inline void UnLock2MB(void* address){
        auto size = UniversalUnLock2MB(address);
        LockedMemory -= size;
        AvailableMemory += size;
    }

    static strict_inline void UnLock2MB(void* address, size_t times){
        for (size_t i = 0; i < times; i++){
            auto size = UniversalUnLock2MB((void*)((u64)address + (i << 21UL)));
            AvailableMemory -= size;
            LockedMemory += size;
        }
    }

    static strict_inline void UnLock1GB(void *address){
        auto size = UniversalUnLock1GB(address);
        LockedMemory -= size;
        AvailableMemory += size;
    }

public:
    static void *Get1GB();
    static void *Get2MB();
    static void *Get4KB();

private:
    static size_t Init(void *location, size_t availableMemory, size_t TotalMemory);

public:
    static size_t SetUp();
};


#endif /* PhisicalAllocator_hpp */
