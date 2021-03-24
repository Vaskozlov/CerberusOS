#include <pci.hpp>
#include <PhisicalAllocator.hpp>
#include <VMManager.hpp>
#include <printf/Printf.h>

extern VMManager KernelVMM;

namespace PCI{

    void EnumerateFunction(u64 deviceAddress, u64 function){
        u64 offset = function << 12;
        u64 functionAddress = deviceAddress + offset;

        KernelVMM.MapMemory((void*)functionAddress, (void*)functionAddress);
        DeviceHeader *pciDeviceHeader = (DeviceHeader*)functionAddress;
        
        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID  == 0xFFFF) return;

        Printf("%s %s\n",
            GetVendorName(pciDeviceHeader->VendorId),
            GetSubclassName(pciDeviceHeader->Class, pciDeviceHeader->SubClass)
        );
    }

    void EnumerateDevice(u64 busAddress, u64 device){
        u64 offset = device << 15;
        u64 deviceAddress = busAddress + offset;

        KernelVMM.MapMemory((void*)deviceAddress, (void*)deviceAddress);

        DeviceHeader *pciDeviceHeader = (DeviceHeader*)deviceAddress;
        
        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID  == 0xFFFF) return;

        for (u64 function = 0; function < 8; function++){
            EnumerateFunction(deviceAddress, function);
        }
    }

    void EnumerateBUS(u64 baseAddress, u64 bus){
        u64 offset = bus << 20;
        u64 busAddress = baseAddress + offset;

        KernelVMM.MapMemory((void*)busAddress, (void*)busAddress);

        DeviceHeader *pciDeviceHeader = (DeviceHeader*)busAddress;
        
        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID  == 0xFFFF) return;

        for (u64 device = 0; device < 32; device++){
            EnumerateDevice(busAddress, device);
        }
    }

    void EnumeratePCI(ACPI::MCFGHeader *mcfg){
        int enteries = ((mcfg->header.length) - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);

        for (int i = 0; i < enteries; i++){
            ACPI::DeviceConfig *newDeviceConfig = (ACPI::DeviceConfig*)((u64)mcfg + sizeof(ACPI::MCFGHeader) + sizeof(ACPI::DeviceConfig) * i);

            for (u64 bus = newDeviceConfig->startBus; bus < newDeviceConfig->endBus; bus++){
                EnumerateBUS(newDeviceConfig->baseAddress, bus);
            }
        }

    }
}
