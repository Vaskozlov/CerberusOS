#ifndef acpi_hpp
#define acpi_hpp

#include <kernel.h>

namespace ACPI{

    struct RSDP2{
        u8 signature[8];
        u8 ckeckSum;
        u8 OEMId[6];
        u8 revision;
        u32 RSDTAddress;
        u32 length;
        u64 XSDTAddress;
        u8 extendedCheckSum;
        u8 reserved[3];
    } __attribute__((packed));

    struct SDTHeader{
        u8 signature[4];
        u32 length;
        u8 revision;
        u8 checkSum;
        u8 OEMId[6];
        u8 OEMTableID[8];
        u32 OEMRevision;
        u32 craetorID;
        u32 craetorRevision;
    } __attribute__((packed));

    struct MCFGHeader{
        SDTHeader header;
        u64 reserver;  
    } __attribute__((packed));

    struct DeviceConfig{
        u64 baseAddress;
        u16 PCISegGroup;
        u8 startBus;
        u8 endBus;
        u32 reserved;
    } __attribute__((packed));

    void *FindTable(SDTHeader *sdtHeader, char *signature);

}

#endif /* acpi_hpp */
