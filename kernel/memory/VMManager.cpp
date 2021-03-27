#include <string.h>
#include "VMManager.hpp"

extern kernel_services_t *KS;

void VMManager::MapMemory4KB(void *virtualMemory, void *PhysicalAddress){
    PageTable *PT;
    PageTable *PD;
    PageTable *PDP;
    PageDirectoryEntry PDE;
    PageMapIndexer4KBMPL4 indexer((u64)virtualMemory);
    
    PDE = PML4->entries[indexer.PDP_i];
    
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false){
        PDP = (PageTable *) PhisicalAllocator::Get4KB();
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
        PD = (PageTable*) PhisicalAllocator::Get4KB();
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
        PT = (PageTable*) PhisicalAllocator::Get4KB();
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

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == true) return;

    PDE.SetAddress(PhysicalAddress);
    PDE.SetFlag(PageDirectoryFlags::present);
    PDE.SetFlag(PageDirectoryFlags::readWrite);
    PD->entries[indexer.PT_i] = PDE;
}

void VMManager::MapMemory2MB(void *virtualMemory, void *PhysicalAddress){
    PageTable *PD;
    PageTable *PDP;
    PageDirectoryEntry PDE;
    PageMapIndexer2MBPML4 indexer((u64)virtualMemory);
    
    PDE = PML4->entries[indexer.PDP_i];
    
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false){
        PDP = (PageTable *) PhisicalAllocator::Get4KB();
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
        PD = (PageTable*) PhisicalAllocator::Get4KB();
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

void VMManager::MapMemory1GB(void *virtualMemory, void *PhysicalAddress){
    PageTable *PDP;
    PageDirectoryEntry PDE;
    PageMapIndexer1GBPML4 indexer((u64)virtualMemory);

    PDE = PML4->entries[indexer.PDP_i];
    
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false){
        PDP = (PageTable *) PhisicalAllocator::Get4KB();
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

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == true) return;

    PDE.SetAddress(PhysicalAddress);
    PDE.SetFlag(PageDirectoryFlags::present);
    PDE.SetFlag(PageDirectoryFlags::readWrite);
    PDE.SetFlag(PageDirectoryFlags::largePage);
    PDP->entries[indexer.PD_i] = PDE;
}
