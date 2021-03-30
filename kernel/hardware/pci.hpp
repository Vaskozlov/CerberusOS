#ifndef pci_hpp
#define pci_hpp

#include <kernel.h>
#include <hardware/acpi.hpp>

namespace PCI{

    struct DeviceHeader{
        u16     VendorId;
        u16     DeviceID;
        u16     Command;
        u16     Status;
        u8      RevisionID;
        u8      ProgIF;
        u8      SubClass;
        u8      Class;
        u8      CacheLineSize;
        u8      LatencyTimes;
        u8      HeaderType;
        u8      BIST;
    };

    struct PCIHeader0{
        DeviceHeader header;
        u32 BAR0;
        u32 BAR1;
        u32 BAR2;
        u32 BAR3;
        u32 BAR4;
        u32 BAR5;
        u32 cardVusCISPtr;
        u16 subSystemVendorID;
        u16 subSystemID;
        u32 expansionROBMBaseAddr;
        u8 capabilitiesPtr;
        u8 reserved0;
        u16 reserved1;
        u32 reserved2;
        u8 interruptLine;
        u8 interruptPin;
        u8 minGrant;
        u8 MaxLatancy;
    };

    extern const char *DeviceClasses[];
    const char *GetVendorName(u16 vendorID);
    const char *GetDeviceName(u16 vendorID, u16 deviceID);
    const char *GetSubclassName(u8 classCode, u8 subclassCode);

    void EnumeratePCI(ACPI::MCFGHeader *mcfg);
}

#endif /* pci_hpp */
