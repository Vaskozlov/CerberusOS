#ifndef pci_hpp
#define pci_hpp

#include <kernel.h>
#include <acpi.hpp>

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

    extern const char *DeviceClasses[];
    const char *GetVendorName(u16 vendorID);
    const char *GetDeviceName(u16 vendorID, u16 deviceID);
    const char *GetSubclassName(u8 classCode, u8 subclassCode);

    void EnumeratePCI(ACPI::MCFGHeader *mcfg);
}

#endif /* pci_hpp */
