#ifndef pci_hpp
#define pci_hpp

#include <kernel.h>
#include <hardware/acpi.hpp>

namespace PCI{

    struct DeviceHeader {
        u16             VendorID;
        u16             DeviceID;
        u16             Command;
        u16             Status;
        u8              RevisionID;
        u8              ProgIF;
        u8              Subclass;
        u8              Class;
        u8              CacheLineSize;
        u8              LatencyTimer;
        u8              HeaderType;
        u8              BIST;
    };

    struct PCIHeader0 {
        DeviceHeader    Header;
        u32             BAR0;
        u32             BAR1;
        u32             BAR2;
        u32             BAR3;
        u32             BAR4;
        u32             BAR5;
        u32             CardbusCISPtr;
        u16             SubsystemVendorID;
        u16             SubsystemID;
        u32             ExpansionROMBaseAddr;
        u8              CapabilitiesPtr;
        u8              Rsv0;
        u16             Rsv1;
        u32             Rsv2;
        u8              InterruptLine;
        u8              InterruptPin;
        u8              MinGrant;
        u8              MaxLatency;
    };

    extern const char *DeviceClasses[];
    const char *GetVendorName(u16 vendorID);
    const char *GetDeviceName(u16 vendorID, u16 deviceID);
    const char *GetSubclassName(u8 classCode, u8 subclassCode);

    void EnumeratePCI(ACPI::MCFGHeader *mcfg);
}

#endif /* pci_hpp */
