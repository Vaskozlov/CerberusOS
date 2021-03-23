#ifndef PhisicalAllocator_hpp
#define PhisicalAllocator_hpp

#include <kernel.h>
#include <BitMap.hpp>

enum EFI_MEMORY_TYPES{
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
    static u64          LastAccess;
    static u64          TotalMemory;
    static u64          AvailableMemory;
    static u64          UsedMemory;
    static u64          ReservedMemory;
    static u64          mMapEnteries;
    static u64          PagesForBitMap;
    static u64          *KernelStart;
    static u64          *KernelEnd;
    static void         *MainMemorySegment;
    static BitMap<u64>  PageBitMap;
    static const char   *EFI_MEMORY_TYPE_STRING[];

public:
    inline static u64 GetTotalMemory()      {   return TotalMemory;       }
    inline static u64 GetAvailableMemory()  {   return AvailableMemory;   }
    inline static u64 GetUsedMemory()       {   return UsedMemory;        }
    inline static u64 GetReservedMemory()   {   return ReservedMemory;    }
    inline static u64 GetmMapEnteries()     {   return mMapEnteries;      }
    inline static u64 GetPagesForBitMap()   {   return PagesForBitMap;    }
    inline static u64 *GetKernelStart()     {   return KernelStart;       }
    inline static u64 *GetKernelEnd()       {   return KernelEnd;         }
    inline static void *GetMainMemorySegment() { return MainMemorySegment; }
    
    inline static const char *GetEfiTypeString(u16 type) {  
        return type < (u16) EfiUknownMemory ? EFI_MEMORY_TYPE_STRING[type] : EFI_MEMORY_TYPE_STRING[EfiUknownMemory];
    }
    
private:
    static void Free(void *address);
    static void Free(void *address, size_t numberOFpages);

    static void Lock(void *address);
    static void Lock(void *address, size_t numberOFpages);

public:
    static void Reserve(void *address);
    static void Reserve(void *address, size_t numberOFpages);

    static void Release(void *address);
    static void Release(void *address, size_t numberOFpages);

    static void *Get();

public:
    static void Init();

public:
    PhisicalAllocator()     = delete;
    ~PhisicalAllocator()    = delete;

};

#endif /* PhisicalAllocator_hpp */
