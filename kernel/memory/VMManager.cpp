#include <string.h>
#include "VMManager.hpp"
#include <printf/Printf.h>

extern kernel_services_t *KS;

PageDirectoryFlags VMManager::GetPageFLags(void *virtualMemory){
    PageTable *PEntry;
    PageDirectoryEntry PDE;
    PageMapIndexer4KBMPL4 indexer((u64)virtualMemory);

    PDE = this->PML4->entries[indexer.PDP_i];
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return NotPresented;
    
    PEntry = (PageTable*)PDE.GetAddress();
    PDE = PEntry->entries[indexer.PD_i];
    
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return NotPresented;
    if (PDE.IsFlagSet(PageDirectoryFlags::largePage) == true) return (PageDirectoryFlags)(PDE.GetFlags() | PageSize1GB);

    PEntry = (PageTable*)PDE.GetAddress();
    PDE = PEntry->entries[indexer.PT_i];
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return NotPresented;
    if (PDE.IsFlagSet(PageDirectoryFlags::largePage) == true) return (PageDirectoryFlags)(PDE.GetFlags() | PageSize2MB);

    PEntry = (PageTable*)PDE.GetAddress();
    PDE = PEntry->entries[indexer.P_i];

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return NotPresented;
    return (PageDirectoryFlags)(PDE.GetFlags() | PageSize4KB);
}

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
    PT->entries[indexer.P_i] = PDE;
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

void VMManager::UnMapMemory4KB(void *virtualMemory, void *PhysicalAddress){
    PageTable *PT;
    PageTable *PD;
    PageTable *PDP;
    PageDirectoryEntry PDE;
    PageMapIndexer4KBMPL4 indexer((u64)virtualMemory);
    
    PDE = PML4->entries[indexer.PDP_i];
    
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;
    else
        PDP = (PageTable*)PDE.GetAddress();

    PDE = PDP->entries[indexer.PD_i];

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;
    else
        PD = (PageTable*) PDE.GetAddress();

    PDE = PD->entries[indexer.PT_i];
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;
    else
        PT = (PageTable*) PDE.GetAddress();

    PDE = PT->entries[indexer.P_i];

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;

    PDE.SetAddress(PhysicalAddress);
    PDE.RemoveFlags();
    PT->entries[indexer.P_i] = PDE;
}

void VMManager::UnMapMemory2MB(void *virtualMemory, void *PhysicalAddress){
    PageTable *PD;
    PageTable *PDP;
    PageDirectoryEntry PDE;
    PageMapIndexer2MBPML4 indexer((u64)virtualMemory);
    
    PDE = PML4->entries[indexer.PDP_i];
    
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;
    else
        PDP = (PageTable*)PDE.GetAddress();

    PDE = PDP->entries[indexer.PD_i];

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;
    else
        PD = (PageTable*) PDE.GetAddress();
   
    PDE = PD->entries[indexer.PT_i];

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;

    PDE.SetAddress(PhysicalAddress);
    PDE.RemoveFlags();
    PD->entries[indexer.PT_i] = PDE;
}

void VMManager::UnMapMemory1GB(void *virtualMemory, void *PhysicalAddress){
    PageTable *PDP;
    PageDirectoryEntry PDE;
    PageMapIndexer1GBPML4 indexer((u64)virtualMemory);

    PDE = PML4->entries[indexer.PDP_i];
    
    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;
    else
        PDP = (PageTable*)PDE.GetAddress();

    PDE = PDP->entries[indexer.PD_i];

    if (PDE.IsFlagSet(PageDirectoryFlags::present) == false) return;

    PDE.SetAddress(PhysicalAddress);
    PDE.RemoveFlags();
    PDP->entries[indexer.PD_i] = PDE;
}


void PrintPDE(PageDirectoryFlags flags){
    
    static char *SizeNames[] = {
        (char*)"4 KB page",
        (char*)"2 MB page",
        (char*)"1 GB page",
        (char*)"Undefined page"
    };

    char *currentSize;

    switch (flags & (PageDirectoryFlags::PageSize4KB | PageDirectoryFlags::PageSize2MB | PageDirectoryFlags::PageSize1GB))
    {
        case PageDirectoryFlags::PageSize4KB:
            currentSize = SizeNames[0];
            break;

        case PageDirectoryFlags::PageSize2MB:
            currentSize = SizeNames[1];
            break;

        case PageDirectoryFlags::PageSize1GB:
            currentSize = SizeNames[2];
            break;

        default:
            currentSize = SizeNames[3];
            break;
    }

    kprintf(
        "Presented: %u\n"
        "readWrite: %u\n"
        "superuser: %u\n"
        "writeThrough: %u\n"
        "Size: %s\n",
        (flags & PageDirectoryFlags::present) > 0,
        (flags & PageDirectoryFlags::readWrite) > 0,
        (flags & PageDirectoryFlags::superuser) > 0,
        (flags & PageDirectoryFlags::writeThrough) > 0,
        currentSize
    );
}
