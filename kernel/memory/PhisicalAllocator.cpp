#include <arch.hpp>
#include <printf/Printf.h>
#include <cerberus/printf.h>
#include "PhisicalAllocator.hpp"

extern u64 _kernelStart;
extern u64 _KernelEnd;
extern kernel_services_t *KS;

u64 PhisicalAllocator::ReservedMemory;
u64 PhisicalAllocator::LockedMemory;
u64 PhisicalAllocator::AllocatorHead;
u64 PhisicalAllocator::TotalMemory;
u64 PhisicalAllocator::AvailableMemory;
u64 PhisicalAllocator::mMapEnteries;

u64 PhisicalAllocator::KernelStart  = (u64)&_kernelStart;
u64 PhisicalAllocator::KernelEnd    = (u64)&_KernelEnd;

cerb::DoubleBitmapFree<u64>         PhisicalAllocator::BigEnteries;
cerb::DoubleBitmapConst<u64, 512>   *PhisicalAllocator::MiddleEntries;
cerb::BitmapConst<u64, 512>         *PhisicalAllocator::SmallEntries;

const char *PhisicalAllocator::EFI_MEMORY_TYPE_STRING[] = {
    "EfiReserverMemory",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOSpace",
    "EfiPalCode",
    "EfiUknownMemory"
};

template<typename T>
always_inline static u64 GetGB(T memsize) { return (u64)memsize >> 30UL ; }

template<typename T>
always_inline static u64 GetMB(T memsize) { return (u64)memsize >> 20UL ; }

template<typename T>
always_inline static u64 Get2MBIndex(T memsize) { return ((u64)memsize >> 21UL) % 512; }

template<typename T>
always_inline static u64 Get4KBIndex(T memsize) { return ((u64)memsize >> 12UL) % 512; }

size_t PhisicalAllocator::UniversalLock4KB(void* address){
    size_t bigIndex = GetGB(address);
    size_t index = Get2MBIndex(address);
    size_t smallIndex = Get4KBIndex(address);

    cerb::DoubleBitmapConst<u64, 512>* header = MiddleEntries + bigIndex;
    cerb::BitmapConst<u64, 512> *smallHeader = SmallEntries + bigIndex * 512 + index;
    
    if ((u64)address > TotalMemory)        return 0;
    if (BigEnteries.at1(bigIndex) == 1)    return 0;
    if (header->at1(index) == 1)           return 0;
    if (smallHeader->at(smallIndex) == 1)  return 0;

    BigEnteries.set2(bigIndex, 1);
    header->set2(index, 1);
    smallHeader->set(smallIndex, 1);
    
    return 0x1000;
}

size_t PhisicalAllocator::UniversalLock2MB(void* address){
    size_t bigIndex = GetGB(address);
    size_t index = Get2MBIndex(address);
    cerb::DoubleBitmapConst<u64, 512>* header = MiddleEntries + bigIndex;
    
    if ((u64)address >= TotalMemory)        return 0;
    if (BigEnteries.at1(bigIndex) == 1)    return 0;
    if (header->at1(index) == 1)           return 0;
    header->set1(index, 1);

    return (1<<21UL);
}

size_t PhisicalAllocator::UniversalLock1GB(void *address){
    size_t index = GetGB(address);

    if (index >= BigEnteries.size()) return 0;
    if (BigEnteries.at1(index) == 1) return 0;

    BigEnteries.set1(index, 1);

    return (1<<30UL);
}

size_t PhisicalAllocator::UniversalUnLock4KB(void* address){
    size_t bigIndex = GetGB(address);
    size_t index = Get2MBIndex(address);
    size_t smallIndex = Get4KBIndex(address);

    cerb::BitmapConst<u64, 512> *smallHeader = SmallEntries + bigIndex * 512 + index;
    
    if ((u64)address > TotalMemory) return 0;
    if (smallHeader->at(smallIndex) == 0) return 0;

    smallHeader->set(smallIndex, 0);

    return 0x1000;
}

size_t PhisicalAllocator::UniversalUnLock2MB(void* address){
    size_t bigIndex = GetGB(address);
    size_t index = Get2MBIndex(address);
    cerb::DoubleBitmapConst<u64, 512>* header = MiddleEntries + bigIndex;
    
    if ((u64)address >= TotalMemory)    return 0;
    if (header->at1(index) == 0)       return 0;

    AvailableMemory += (1<<21UL);
    LockedMemory -= (1<<21UL);
    
    header->set1(index, 0);
    return (1<<21UL);
}

size_t PhisicalAllocator::UniversalUnLock1GB(void *address){
    size_t bigIndex = GetGB(address);

    if (bigIndex >= BigEnteries.size())     return 0;
    if (BigEnteries.at1(bigIndex) == 0)    return 0;

    AvailableMemory += (1<<30UL);
    LockedMemory -= (1<<30UL);
    BigEnteries.set1(bigIndex, 0);

    return (1<<30UL);
}

void *PhisicalAllocator::Get4KB(){
    size_t index = BigEnteries.findFirstFreeAndSecondSet();//findFree1not0();

    if (index == UINTMAX_MAX){
        index = BigEnteries.findFree1();
        if (index == UINTMAX_MAX) return NULL;

        BigEnteries.set2(index, 1);
        cerb::DoubleBitmapConst<u64, 512>* header = MiddleEntries + index;
        
        u64 middleIndex = header->findFree2();
        header->set2(middleIndex, 1);

        cerb::BitmapConst<u64, 512> *smallHeader = SmallEntries + index * 512 + middleIndex;
       
        u64 smallIndex = smallHeader->findFree();
        smallHeader->set(smallIndex, 1);

        AvailableMemory -= 0x1000;
        LockedMemory += 0x1000;

        return (void*)((index << 30) + (middleIndex << 21) + (smallIndex << 12));
    }

    cerb::DoubleBitmapConst<u64, 512>* header = MiddleEntries + index;
    size_t middleIndex = header->findFirstFreeAndSecondSet();//findFree1not0();

    if (middleIndex == UINTMAX_MAX){
        middleIndex = header->findFree1();
        
        if (middleIndex == UINTMAX_MAX) return NULL;

        header->set1(middleIndex, 1);
    }
    
    cerb::BitmapConst<u64, 512> *smallHeader = SmallEntries + index * 512 + middleIndex;
    size_t smallIndex = smallHeader->findFree();

    if (smallIndex == UINTMAX_MAX){
        header->set1(middleIndex, 1);
        return Get4KB();
    }

    AvailableMemory -= 0x1000;
    LockedMemory += 0x1000;
    smallHeader->set(smallIndex, 1);

    return (void*)((index << 30) + (middleIndex << 21) + (smallIndex << 12));
}

void *PhisicalAllocator::Get2MB(){
    size_t index = BigEnteries.findFirstFreeAndSecondSet();//findFree1not0();
    cerbPrintf("index = %u\n", index);

    if (index == UINTMAX_MAX){
        index = BigEnteries.findFree1();
        if (index == UINTMAX_MAX) return NULL;

        BigEnteries.set2(index, 1);
        cerb::DoubleBitmapConst<u64, 512>* header = MiddleEntries + index;
        
        u64 middleIndex = header->findFree1();
        header->set1(middleIndex, 1);

        AvailableMemory -= (1<<21UL);
        LockedMemory += (1<<21UL);

        return (void*)((index << 30) + (middleIndex << 21));
    }

    cerb::DoubleBitmapConst<u64, 512>* header = MiddleEntries + index;
    cerbPrintf("MiddelEnteries (location): %p\n", MiddleEntries);
    
    u64 middleIndex = header->findFirstSetAndSecondFree(); //findFree0not1();
    cerbPrintf("middleIndex = %u\n", middleIndex);
    header->set1(middleIndex, 1);

    AvailableMemory -= (1<<21UL);
    LockedMemory += (1<<21UL);

    return (void*)((index << 30) + (middleIndex << 21));
}

void *PhisicalAllocator::Get1GB(){

    size_t index = BigEnteries.findFree1And2();//findFree0Is0And1Is0();
    
    if (index == UINTMAX_MAX) return NULL;

    BigEnteries.set1(index, 1);

    LockedMemory += (1<<30UL);
    AvailableMemory -= (1<<30UL);

    return (void*)(index << 30);
}


size_t PhisicalAllocator::Init(void *location, size_t availableMemory, size_t totalMemory){
    TotalMemory = totalMemory;
    AvailableMemory = availableMemory;
    LockedMemory = 0;

    BigEnteries = cerb::move(
            cerb::DoubleBitmapFree<u64>(
                (u64*)location,
                cerb::MAX<size_t>(totalMemory >> 30 + (totalMemory % ((1<<30UL) - 1) > 0), 1)
            )
        );
    AllocatorHead = (u64)location + cerb::align(BigEnteries.size(), 6) / sizeof(u64) * 2;
    BigEnteries.clear();

    MiddleEntries = (cerb::DoubleBitmapConst<u64, 512>*)AllocatorHead;
    AllocatorHead += cerb::MAX<size_t>(
            BigEnteries.size() * sizeof(cerb::DoubleBitmapConst<u64, 512>),
            16
        );

    cerbPrintf("Number of enteries (middle): %u\n", (totalMemory >> 21) + 1);
    cerbPrintf("Change (middle): %lu\n", BigEnteries.size());
    
    SmallEntries = (cerb::BitmapConst<u64, 512>*)AllocatorHead;
    AllocatorHead += cerb::MAX<size_t>(
        BigEnteries.size() * 512 * sizeof(cerb::BitmapConst<u64, 512>),
        8
    );
    cerbPrintf("Number of enteries (small): %lu\n", (totalMemory / 0x1000) + 1);
    cerbPrintf("Change (small): %lu\n", BigEnteries.size() * 512 * sizeof(cerb::BitmapConst<u64, 512>));

    cerbPrintf("Memset from %p to %p (%lu times)\n", location, AllocatorHead, (AllocatorHead - (u64)location) / sizeof(u64) + 1);
    ARCH::memset64(location, 0UL, (AllocatorHead - (u64)location) / sizeof(u64) + 1);
    return AllocatorHead - (u64)location;
}

size_t PhisicalAllocator::SetUp(){
    if (mMapEnteries > 0) return 0;
    
    void *mainMemory = NULL;
    u64 largestSegment = 0;

    mMapEnteries = KS->mMapSize / KS->mMapDescriptorSize;

    for (u64 i = 0; i < mMapEnteries; i++){
        MemoryDescriptor_t *descriptor = (MemoryDescriptor_t *)((u64)KS->mMap + i * KS->mMapDescriptorSize);

        if (descriptor->type == EFI_MEMORY_TYPE::EfiConventionalMemory){
            AvailableMemory += descriptor->numberOfPages;
            
            if (descriptor->numberOfPages > largestSegment){
                largestSegment = descriptor->numberOfPages;
                mainMemory = descriptor->physicalStart;
            }
        }

        TotalMemory += descriptor->numberOfPages;
    }
    
    TotalMemory *= 0x1000;
    AvailableMemory *= 0x1000;

    auto initSize = Init(mainMemory, AvailableMemory, TotalMemory);
    PhisicalAllocator::Lock4KB(0x0); // skip first 0x1000
    
    {
        i64 selfSize2MB = initSize >> 21;
        i64 selfSize4KB = initSize >> 12;
        size_t lockedBytes = 0;

        while (selfSize2MB > 0){
            Lock2MB((void*)((u64)mainMemory + lockedBytes));

            selfSize2MB--;
            selfSize4KB-= 512;
            lockedBytes += (1<<21UL);
        }

        while (selfSize4KB > 0){
            Lock4KB((void*)((u64)mainMemory + lockedBytes));

            selfSize4KB--;
            lockedBytes += (1<<12UL);
        }
    }

    for (size_t i = 0; i < mMapEnteries; i++){
        MemoryDescriptor_t *descriptor = (MemoryDescriptor_t *)((u64)KS->mMap + i * KS->mMapDescriptorSize);

        if (descriptor->type != EFI_MEMORY_TYPE::EfiConventionalMemory){
            i64 pages4KB = descriptor->numberOfPages;
            i64 pages2MB = pages4KB / 512;
            u64 location = (u64)descriptor->physicalStart;

            while (pages2MB > 0){
                Lock2MB((void*)location);

                location += (1<<21UL);
                pages2MB--;
                pages4KB -= 512;
            }

            while (pages4KB > 0){
                Lock4KB((void*)location);

                location += (1<<12UL);
                pages4KB--;
            }
        }
    }

    Lock4KB((void*)KernelStart, ((u64)KernelEnd - (u64)KernelStart) / 0x1000);
    Lock4KB(KS->frameBuffer.base_address, KS->frameBuffer.buffer_size / 0x1000 + 1);

    return initSize;
} //38f4
