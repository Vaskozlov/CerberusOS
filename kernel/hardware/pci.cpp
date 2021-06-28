#include "pci.hpp"
#include "memory/VMManager.hpp"
#include "ahci/ahci.hpp"
#include <memory/kmalloc.h>
#include <cerberus/printf.h>
#include <interrupts/Interrupts.h>
#include <array>
#include <cerberus/vector.hpp>

extern VMManager KernelVMM;

namespace PCI{

    void EnumerateFunction(u64 deviceAddress, u64 function){
        u64 offset = function << 12;
        u64 functionAddress = deviceAddress + offset;

        PCIDevice *pciDeviceHeader = (PCIDevice*)functionAddress;
        
        if (pciDeviceHeader->deviceID() == 0x0000) return;
        if (pciDeviceHeader->deviceID() == 0xFFFF) return;
       
        switch (pciDeviceHeader->getClass()) {

            case 0x01: // Mass Storage Controller
                switch (pciDeviceHeader->subclass()) {
                    case 0x06:
                        switch (pciDeviceHeader->progIF()) {
                            case 0x1: // AHCI 1.0
                                //new AHCI::AHCIDriver(pciDeviceHeader);
                                break;
                        }
                        break;
                }
                break;
            
            case 0x0C: // Serial Bus Controller
                switch (pciDeviceHeader->subclass()){
                    case 0x03: // USB Controller
                        switch (pciDeviceHeader->progIF()){

                            /*

                                if it's memory mapped I/O we have to align it to 16 and save 4 last bits
                                see location 1044

                            */

                            case 0x00: // UHCI Controller 
                            {
                                PCIHeader0 *header = (PCIHeader0 *)pciDeviceHeader;
                                u32 *IO_ADDR = (u32*)(u64)header->BAR4;
                                
                                u32 value = *IO_ADDR;
                                *IO_ADDR = 0xFFFFFFFF;
                                u32 result = *IO_ADDR;
                                *IO_ADDR = value;
                                header->_command = 0x06;
                                header->InterruptLine = 0x30;

                                *(u16*)((u8*)header + 0xC0) = 0x8f00;
                                cerbPrintf("%p\n", (u16)((~(result & ~0b1)) + 1));
                                cerbPrintf("UHCI Controller found!\n");
                            } break;

                            case 0x10: // OHCI Controller
                            {
                                PCIHeader0 *header = (PCIHeader0 *)pciDeviceHeader;
                                u32 *IO_ADDR = (u32*)(u64)header->BAR0;
                                
                                u32 value = *IO_ADDR;
                                *IO_ADDR = 0xFFFFFFFF;
                                u32 result = *IO_ADDR;
                                *IO_ADDR = value;
                                header->_command = 0x06;
                                header->InterruptLine = 0x30;

                                cerbPrintf("%p\n", (u16)((~(result & ~0b1)) + 1));
                                cerbPrintf("OHCI Controller found!\n");
                            } break;

                            case 0x20: // EHCI (USB2) Controller 
                            {
                                PCIHeader0 *header = (PCIHeader0 *)pciDeviceHeader;
                                u32 *IO_ADDR = (u32*)(u64)header->BAR0;
                                
                                u32 value = *IO_ADDR;
                                *IO_ADDR = 0xFFFFFFFF;
                                u32 result = *IO_ADDR;
                                *IO_ADDR = value;
                                header->_command = 0x06;
                                header->InterruptLine = 0x30;

                                cerbPrintf("%p\n", (u16)((~(result & ~0b1)) + 1));
                                cerbPrintf("EHCI (USB2) Controlle found!\n");
                            } break;

                            case 0x30: // XHCI (USB3) Controller 
                            {
                                cerbPrintf("XHCI (USB3) Controller  found!\n");
                            } break;

                            default:
                                LOG("Unspecified USB device");
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

        PCIDevice *pciDeviceHeader = (PCIDevice*)deviceAddress;
        
        if (pciDeviceHeader->deviceID() == 0x0000) return;
        if (pciDeviceHeader->deviceID() == 0xFFFF) return;

        for (u64 function = 0; function < 8; function++)
            EnumerateFunction(deviceAddress, function);
    }

    void EnumerateBUS(u64 baseAddress, u64 bus){
        u64 offset = bus << 20;
        u64 busAddress = baseAddress + offset;

        PCIDevice *pciDeviceHeader = (PCIDevice*)busAddress;
        
        if (pciDeviceHeader->deviceID() == 0x0000) return;
        if (pciDeviceHeader->deviceID() == 0xFFFF) return;

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
