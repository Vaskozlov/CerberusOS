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
    PageMapIndexerLVL3 indexer((u64)virtualMemory);
    
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

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == true) return;

    PDE.SetAddress(PhysicalAddress);
    PDE.SetFlag(PageDirectoryFlags::present);
    PDE.SetFlag(PageDirectoryFlags::readWrite);
    PDE.SetFlag(PageDirectoryFlags::largePage);
    PD->entries[indexer.PT_i] = PDE;
}
