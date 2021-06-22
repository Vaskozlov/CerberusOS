#ifndef pci_hpp
#define pci_hpp

#include <kernel.h>
#include <hardware/acpi.hpp>

namespace PCI{

    extern const char *DeviceClasses[];
    const char *GetVendorName(u16 vendorID);
    const char *GetDeviceName(u16 vendorID, u16 deviceID);
    const char *GetSubclassName(u8 classCode, u8 subclassCode);

    class PCIDevice{
        u16     _vendorId;
        u16     _deviceID;
        u16     _command;
        u16     _status;
        u8      _revisionID;
        u8      _progIF;
        u8      _subclass;
        u8      _class;
        u8      _cacheLineSize;
        u8      _latencyTimer;
        u8      _headerType;
        u8      _BIST;

    public:
        always_inline auto vendorID()       const -> u16 { return _vendorId     ; }
        always_inline auto deviceID()       const -> u16 { return _deviceID     ; }
        always_inline auto command()        const -> u16 { return _command      ; }
        always_inline auto status()         const -> u16 { return _status       ; }
        always_inline auto revisionID()     const -> u8  { return _revisionID   ; }
        always_inline auto progIF()         const -> u8  { return _progIF       ; }
        always_inline auto subclass()       const -> u8  { return _subclass     ; }
        always_inline auto getClass()       const -> u8  { return _class        ; }
        always_inline auto cacheLineSize()  const -> u8  { return _cacheLineSize; }
        always_inline auto latencyTimer()   const -> u8  { return _latencyTimer ; }
        always_inline auto headerType()     const -> u8  { return _headerType   ; }
        always_inline auto BIST()           const -> u8  { return _BIST         ; }
    
    public:
        const char *getVendor()         const { return GetVendorName(vendorID())                ; }
        const char *getName()           const { return GetDeviceName(vendorID(), deviceID())    ; }
        const char *getSubclassName()   const { return GetSubclassName(getClass(), subclass())  ; }

    } __attribute__((packed));

    struct PCIHeader0 : public PCIDevice{
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

    void EnumeratePCI(ACPI::MCFGHeader *mcfg);
}

#endif /* pci_hpp */
