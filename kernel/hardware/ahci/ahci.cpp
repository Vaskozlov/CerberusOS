#include "ahci.hpp"
#include <printf/Printf.h>
#include <memory/kmalloc.h>
#include <memory/VMManager.hpp>

extern VMManager KernelVMM;

namespace AHCI {

    AHCIDriver::AHCIDriver(PCI::DeviceHeader *pciBaseAddress)
    : PCIDeviceAddress(pciBaseAddress)
    {
        ABAR = (HBAMmeory*)(((PCI::PCIHeader0*)pciBaseAddress)->BAR5);
        
        if ((u64)ABAR > PhisicalAllocator::GetTotalMemory())
            KernelVMM.MapMemory4KB(ABAR, ABAR);

        kprintf("AHCI Driver ready\n");
    }

    AHCIDriver::~AHCIDriver(){
        
    }
}
