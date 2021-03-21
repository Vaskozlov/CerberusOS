#include <memory.hpp>
#include <printf/Printf.h>
#include <string.h>

extern u64 _edata;
extern kernel_services_t *KS;

u64 pMemoryManager::SystemMemory;
u64 pMemoryManager::AvailableMemory;
u64 pMemoryManager::ReservedMemory;
u64 pMemoryManager::UsedMemory;
u64 pMemoryManager::mMapEnteries;
u64 *pMemoryManager::KernelStart = NULL;
u64 *pMemoryManager::KernelEnd  = &_edata;
BitMap pMemoryManager::PageBitMap;


const char *pMemoryManager::EFI_MEMORY_TYPE_STRING[] = {
    "EfiReserverMemory",
    "EfiLoaderCode",
    "EfiLoaderDara",
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
    "EfiPalCode"
};

void pMemoryManager::FreePage(void *address){
    size_t index = (size_t)address / 0x1000;

    if (PageBitMap[index] == 0) return;

    PageBitMap.set(index, 0);

    AvailableMemory += 0x1000;
    UsedMemory -= 0x1000;
}

void pMemoryManager::FreePages(void *address, size_t amount){
    for (size_t index = 0; index < amount; index++){
        FreePage((void*)((u64) address + 0x1000 * index));
    }
}

void pMemoryManager::LockPage(void *address){
    size_t index = (size_t)address / 0x1000;

    if (PageBitMap[index] == 1) return;

    PageBitMap.set(index, 1);

    AvailableMemory -= 0x1000;
    UsedMemory += 0x1000;
}

void pMemoryManager::LockPages(void *address, size_t amount){
    for (size_t index = 0; index < amount; index++){
        LockPage((void*)((u64) address + 0x1000 * index));
    }
}

void pMemoryManager::Reserve(void *address){
    size_t index = (size_t)address / 0x1000;

    if (PageBitMap[index] == 1) return;

    PageBitMap.set(index, 1);

    AvailableMemory -= 0x1000;
    ReservedMemory += 0x1000;
}

void pMemoryManager::UnReserve(void *address){
    size_t index = (size_t)address / 0x1000;

    if (PageBitMap[index] == 0) return;

    PageBitMap.set(index, 0);

    AvailableMemory += 0x1000;
    ReservedMemory -= 0x1000;
}

void pMemoryManager::Reserve(void *address, size_t amount){
    for (size_t index = 0; index < amount; index++){
        Reserve((void*)((u64) address + 0x1000 * index));
    }
}

void pMemoryManager::UnReserve(void *address, size_t amount){
    for (size_t index = 0; index < amount; index++){
        UnReserve((void*)((u64) address + 0x1000 * index));
    }
}

void *pMemoryManager::RequestPage(){
    for (size_t i = 0; i < PageBitMap.size(); i++){
        if (PageBitMap[i] == 1) continue;

        LockPage((void*)(i * 0x1000));
        return (void*)(i * 0x1000);
    }
    return NULL;
}

void pMemoryManager::init(){

    if (mMapEnteries > 0) return;

    size_t maxEneterSize = 0;
    void *largestMemorySegment = NULL;
    mMapEnteries = KS->mMapSize / KS->mMapDescriptorSize;

    for (size_t i = 0; i < mMapEnteries; i++){
        MemoryDescriptor_t *descriptor = (MemoryDescriptor_t *)((u64)KS->mMap + i * KS->mMapDescriptorSize);
        
        if (descriptor->type == 7 && descriptor->numberOfPages > maxEneterSize){
            maxEneterSize = descriptor->numberOfPages;
            largestMemorySegment = descriptor->physicalStart;
        }
        SystemMemory += descriptor->numberOfPages * 0x1000;
    }

    AvailableMemory = maxEneterSize * 0x1000;
    u64 bitMapSize = SystemMemory / 4096 / 8 + 1;

    PageBitMap = BitMap((u8*) largestMemorySegment, bitMapSize);
    memset(largestMemorySegment, 0, bitMapSize / 8);

    LockPages(largestMemorySegment, PageBitMap.size() / 4096 + 1);

    for (size_t i = 0; i < mMapEnteries; i++){
        MemoryDescriptor_t *descriptor = (MemoryDescriptor_t *)((u64)KS->mMap + i * KS->mMapDescriptorSize);
        
        if (descriptor->type != 7)
            Reserve(descriptor->physicalStart, descriptor->numberOfPages);
    }

    LockPages(KernelStart, ((u64)KernelEnd - (u64)KernelStart) / 0x1000 + 1);
    LockPages(KS->frameBuffer.base_address, KS->frameBuffer.buffer_size / 0x1000 + 1);
}

PageMapIndexer::PageMapIndexer(u64 VirtualAddress){
    VirtualAddress >>= 12;

    P_i = VirtualAddress & 0x1FF;
    VirtualAddress >>= 9;

    PT_i = VirtualAddress & 0x1FF;
    VirtualAddress >>= 9
    ;
    PD_i = VirtualAddress & 0x1FF;
    VirtualAddress >>= 9;

    PDP_i = VirtualAddress & 0x1FF;
}

void vMemoryManager::mapMemory(void *virtualMemory, void *physicalMemory){
    PageMapIndexer indexer((u64)virtualMemory);
    PageDirectoryEntry PDE;

    PDE = PML4->entries[indexer.PDP_i];
    PageTable *PDP;

    if (PDE.present == false){
        PDP = (PageTable*) pMemoryManager::RequestPage();
        memset(PDP, 0, 0x1000);
        PDE.address = ((u64)PDP >> 12);
        PDE.present = true;
        PDE.readWrite = true;
        PML4->entries[indexer.PDP_i] = PDE;
    }
    else{
        PDP = (PageTable*)(PDE.address << 12);
    }

    PDE = PDP->entries[indexer.PD_i];
    PageTable *PD;

    if (virtualMemory == (void*)0x38000){
        Printf("%PDP: %p\n", PDP);
    }

    if (!PDE.present){
        PD = (PageTable*) pMemoryManager::RequestPage();
        memset(PD, 0, 0x1000);
        PDE.address = ((u64) PD >> 12);
        PDE.present = true;
        PDE.readWrite = true;
        PDP->entries[indexer.PD_i] = PDE;
    }
    else{
        PD = (PageTable*)(PDE.address << 12);
    }

    if (virtualMemory == (void*)0x38000){
        Printf("PD: %p\n", PD);
    }

    PDE = PD->entries[indexer.PT_i];
    PageTable *PT;

    if (!PDE.present){
        PT = (PageTable*) pMemoryManager::RequestPage();
        memset(PT, 0, 0x1000);
        PDE.address = ((u64) PT >> 12);
        PDE.present = true;
        PDE.readWrite = true;
        PD->entries[indexer.PT_i] = PDE;
    }
    else{
        PT = (PageTable*)(PDE.address << 12);
    }

    PDE = PT->entries[indexer.P_i];
    PDE.address = (u64)physicalMemory >> 12;
    PDE.present = true;
    PDE.readWrite = true;
    PT->entries[indexer.P_i] = PDE;
}
