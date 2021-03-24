#include <string.h>
#include <PhisicalAllocator.hpp>
#include <printf/Printf.h>

extern u64 _kernelStart;
extern u64 _KernelEnd;
extern kernel_services_t *KS;

u64         PhisicalAllocator::LastAccess;
u64         PhisicalAllocator::TotalMemory;
u64         PhisicalAllocator::AvailableMemory;
u64         PhisicalAllocator::UsedMemory;
u64         PhisicalAllocator::ReservedMemory;
u64         PhisicalAllocator::mMapEnteries;
u64         PhisicalAllocator::PagesForBitMap;
void        *PhisicalAllocator::MainMemorySegment;
u64         *PhisicalAllocator::KernelStart     = &_kernelStart;
u64         *PhisicalAllocator::KernelEnd       = &_KernelEnd;
BitMap<u64> PhisicalAllocator::PageBitMap;

const char  *PhisicalAllocator::EFI_MEMORY_TYPE_STRING[] = {
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

void PhisicalAllocator::Free(void *address){
    size_t index = (size_t) address / 0x1000;

    if (PageBitMap[index] == 0) return;

    PageBitMap.set(index, 0);

    UsedMemory      -= 0x1000;
    AvailableMemory += 0x1000;

    LastAccess = MIN(LastAccess, index);
}

void PhisicalAllocator::Free(void *address, size_t numberOFpages){
    size_t index = (size_t) address / 0x1000;
    LastAccess = MIN(LastAccess, index);

    for (size_t i = 0; i < numberOFpages; i++, index++){

        if (PageBitMap[index] == 0) continue;

        PageBitMap.set(index, 0);

        UsedMemory      -= 0x1000;
        AvailableMemory += 0x1000;
    }
}

void PhisicalAllocator::Lock(void *address){
    size_t index = (size_t) address / 0x1000;

    if (PageBitMap[index] == 1) return;

    PageBitMap.set(index, 1);

    AvailableMemory -= 0x1000;
    UsedMemory += 0x1000;

    LastAccess = index + 1;
}

void PhisicalAllocator::Lock(void *address, size_t numberOFpages){
    size_t index = (size_t) address / 0x1000;

    for (size_t i = 0; i < numberOFpages; i++, index++){

        if (PageBitMap[index] == 1) continue;
        PageBitMap.set(index, 1);

        UsedMemory      += 0x1000;
        AvailableMemory -= 0x1000;
    }

    LastAccess = index + numberOFpages;
}

void PhisicalAllocator::Reserve(void *address){
    size_t index = (size_t) address;

    if (PageBitMap[index] == 1) return;
    PageBitMap.set(index, 1);

    AvailableMemory -= 0x1000;
    ReservedMemory += 0x1000;

    LastAccess = index + 1;
}

void PhisicalAllocator::Reserve(void *address, size_t numberOFpages){
    size_t index = (size_t) address / 0x1000;

    for (size_t i = 0; i < numberOFpages; i++, index++){
        if (PageBitMap[index] == 1) continue;
        PageBitMap.set(index, 1);

        AvailableMemory -= 0x1000;
        ReservedMemory += 0x1000;
    }

    LastAccess = index + numberOFpages;
}

void PhisicalAllocator::Release(void *address){
    size_t index = (size_t) address;

    if (PageBitMap[index] == 0) return;
    PageBitMap.set(index, 0);

    AvailableMemory += 0x1000;
    ReservedMemory -= 0x1000;

    LastAccess = MIN(index, LastAccess);
}

void PhisicalAllocator::Release(void *address, size_t numberOFpages){
    size_t index = (size_t) address / 0x1000;
    LastAccess = MIN(index, LastAccess);

    for (size_t i = 0; i < numberOFpages; i++, index++){
        if (PageBitMap[index] == 0) continue;
        PageBitMap.set(index, 0);

        AvailableMemory += 0x1000;
        ReservedMemory -= 0x1000;
    }
}

void *PhisicalAllocator::Get(){
    size_t index = PageBitMap.findFree(LastAccess);

    if (index == UINTMAX_MAX) return NULL;

    Lock((void*)(index * 0x1000));    
    return (void*)(index * 0x1000);
}

void PhisicalAllocator::Init(){

    if (mMapEnteries > 0) return;

    void *largestMemory;
    size_t largesrSegment = 0;
    u64 numberOfPages = 0;
    mMapEnteries = KS->mMapSize / KS->mMapDescriptorSize;

    for (size_t i = 0; i < mMapEnteries; i++){
        MemoryDescriptor_t *descriptor = (MemoryDescriptor_t *)((u64)KS->mMap + i * KS->mMapDescriptorSize);
       
        if (
            descriptor->type == EFI_MEMORY_TYPES::EfiConventionalMemory &&
            descriptor->numberOfPages > largesrSegment
        ){
            largestMemory = descriptor->physicalStart;
            largesrSegment = descriptor->numberOfPages;
        }
        numberOfPages += descriptor->numberOfPages;
    }

    AvailableMemory = largesrSegment * 0x1000;
    TotalMemory = numberOfPages * 0x1000;
    MainMemorySegment = largestMemory;
    PagesForBitMap = numberOfPages / 0x1000 / 0x08;

    PageBitMap = BitMap<u64>((u64*) largestMemory, numberOfPages);
    memset(largestMemory, 0, numberOfPages / 0x08);

    Lock(largestMemory, PagesForBitMap);

    for (size_t i = 0; i < mMapEnteries; i++){
        MemoryDescriptor_t *descriptor = (MemoryDescriptor_t *)((u64)KS->mMap + i * KS->mMapDescriptorSize);

        if (descriptor->type != EFI_MEMORY_TYPES::EfiConventionalMemory)
            Reserve(descriptor->physicalStart, descriptor->numberOfPages);
    }
    
    Lock(KernelStart, ((u64)KernelEnd - (u64)KernelStart) / 0x1000);
    Lock(KS->frameBuffer.base_address, KS->frameBuffer.buffer_size / 0x1000 + 1);
}
