#include <string.h>
#include <VMManager.hpp>

extern kernel_services_t *KS;

PageMapIndexer::PageMapIndexer(u64 VirtualAddress){
    VirtualAddress >>= 12;

    P_i = VirtualAddress & 0x1FF;
    VirtualAddress >>= 9;

    PT_i = VirtualAddress & 0x1FF;
    VirtualAddress >>= 9;

    PD_i = VirtualAddress & 0x1FF;
    VirtualAddress >>= 9;

    PDP_i = VirtualAddress & 0x1FF;
}

void VMManager::MapMemory(void *virtualMemory, void *PhysicalAddress){
    PageTable *PT;
    PageTable *PD;
    PageTable *PDP;
    PageDirectoryEntry PDE;
    PageMapIndexer indexer((u64)virtualMemory);
    
    PDE = PML4->entries[indexer.PDP_i];
    
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false){
        PDP = (PageTable *) PhisicalAllocator::Get();
        memset(PDP, 0, 0x1000);
        
        PDE.SetAddress(PDP);
        PDE.SetFlag(PageDirectoryFlags::present);
        PDE.SetFlag(PageDirectoryFlags::readWrite);

        PML4->entries[indexer.PDP_i] = PDE;
    }
    else{
        PDP = (PageTable*)PDE.GetAddress();
    }

    PDE = PDP->entries[indexer.PD_i];

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false){
        PD = (PageTable*) PhisicalAllocator::Get();
        memset(PD, 0, 0x1000);

        PDE.SetAddress(PD);
        PDE.SetFlag(PageDirectoryFlags::present);
        PDE.SetFlag(PageDirectoryFlags::readWrite);

        PDP->entries[indexer.PD_i] = PDE;
    }
    else{
        PD = (PageTable*) PDE.GetAddress();
    }

    PDE = PD->entries[indexer.PT_i];

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false){
        PT = (PageTable*) PhisicalAllocator::Get();
        memset(PT, 0, 0x1000);

        PDE.SetAddress(PT);
        PDE.SetFlag(PageDirectoryFlags::present);
        PDE.SetFlag(PageDirectoryFlags::readWrite);

        PD->entries[indexer.PT_i] = PDE;
    }
    else{
        PT = (PageTable*) PDE.GetAddress();
    }

    PDE = PT->entries[indexer.P_i];
    PDE.SetAddress(PhysicalAddress);
    PDE.SetFlag(PageDirectoryFlags::present);
    PDE.SetFlag(PageDirectoryFlags::readWrite);
    PT->entries[indexer.P_i] = PDE;
}
