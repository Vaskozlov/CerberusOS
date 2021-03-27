#include <kernel.h>

namespace PCI{

    static const char *MassStorageControllerSubclassName[] = {
        "SCSI Bus Controller",
        "IDE Controller",
        "Floppy Disk Controller",
        "IPI Bus Controller",
        "RAID Controller",
        "ATA Controller",
        "Serial ATA",
        "Serial Attached SCSI",
        "Non-Volatile Memory Controller"
    };

    const char *DeviceClasses[] = {
        "Unclassified",
        "Mass Storage Controller",
        "Network Controller",
        "Display Controller",
        "Multimedia Controller",
        "Memory Controller",
        "Bridge Devide",
        "Simple Communication Controller",
        "Base System Preipheral",
        "Input Device Controller",
        "Docking Startion",
        "Processor",
        "Serial Bus Controller",
        "Wireless Controller",
        "Intelligent Controller",
        "Satellite Communication Controller",
        "Encryption Controller",
        "Signal Processing Controller",
        "Processing Accelerator",
        "Non-Essential Instrumenrarion",
    };

    const char *GetVendorName(u16 vendorID){
        
        switch (vendorID) {
            case 0x8086:
                return "Intel Corp";

            case 0x1022:
                return "AMD";

            case 0x10DE:
                return "NVIDIA Corporation";
        }

        return "Unclassified vendor";
    }

    const char *GetDeviceName(u16 vendorID, u16 deviceID){
        switch (vendorID) {
            case 0x8086:
                switch (deviceID){
                    case 0x29c0:
                        return "Express DRAM Controller";
                    case 0x2918:
                        return "LPC Interface Controller";
                    case 0x2922:
                        return "6 port SATA Controller";
                    case 0x2930:
                        return "SMBus Controller";
                }
                break;
        }

        return "Unclassified device";
    }

    const char *GetSubclassName(u8 classCode, u8 subclassCode){

        switch (classCode){
            case 0x01:
                return subclassCode < sizeof(MassStorageControllerSubclassName) / 8 ?
                    MassStorageControllerSubclassName[subclassCode] : "Unclassified";
        }

        return "Unclassified subclass";
    }
}
