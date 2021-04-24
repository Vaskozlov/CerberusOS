#include "pci.hpp"
#include "memory/VMManager.hpp"
#include <printf/Printf.h>
#include "ahci/ahci.hpp"
#include <memory/kmalloc.h>

extern VMManager KernelVMM;

namespace PCI{

    void EnumerateFunction(u64 deviceAddress, u64 function){
        u64 offset = function << 12;
        u64 functionAddress = deviceAddress + offset;

        DeviceHeader *pciDeviceHeader = (DeviceHeader*)functionAddress;
        
        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID  == 0xFFFF) return;
        
        switch (pciDeviceHeader->Class) {

            case 0x01:
                switch (pciDeviceHeader->Subclass) {
                    case 0x06:
                        switch (pciDeviceHeader->ProgIF) {
                            case 0x1: // AHCI 1.0
                                new AHCI::AHCIDriver(pciDeviceHeader);
                                break;
                        }
                        break;
                }
                break;
        }
    }

    void EnumerateDevice(u64 busAddress, u64 device){
        u64 offset = device << 15;
        u64 deviceAddress = busAddress + offset;

        DeviceHeader *pciDeviceHeader = (DeviceHeader*)deviceAddress;
        
        if (pciDeviceHeader->DeviceID == 0)         return;
        if (pciDeviceHeader->DeviceID  == 0xFFFF)   return;

        for (u64 function = 0; function < 8; function++)
            EnumerateFunction(deviceAddress, function);
    }

    void EnumerateBUS(u64 baseAddress, u64 bus){
        u64 offset = bus << 20;
        u64 busAddress = baseAddress + offset;

        DeviceHeader *pciDeviceHeader = (DeviceHeader*)busAddress;
        
        if (pciDeviceHeader->DeviceID == 0)         return;
        if (pciDeviceHeader->DeviceID  == 0xFFFF)   return;

        for (u64 device = 0; device < 32; device++)
            EnumerateDevice(busAddress, device);
    }

    void EnumeratePCI(ACPI::MCFGHeader *mcfg){
        int enteries = ((mcfg->header.length) - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);

        for (int i = 0; i < enteries; i++){
            ACPI::DeviceConfig *newDeviceConfig = (ACPI::DeviceConfig*)((u64)mcfg + sizeof(ACPI::MCFGHeader) + sizeof(ACPI::DeviceConfig) * i);

            for (u64 bus = newDeviceConfig->startBus; bus < newDeviceConfig->endBus; bus++)
                EnumerateBUS(newDeviceConfig->baseAddress, bus);
        }
    }
}
