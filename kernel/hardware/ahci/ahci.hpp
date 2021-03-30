#ifndef ahci_hpp
#define ahci_hpp

#include <kernel.h>
#include <hardware/pci.hpp>

namespace AHCI {

    struct HBAPort{
        u32 commandListBase;
        u32 commandListBaseUpper;
        u32 fisBaseAddres;;
        u32 fisAddressUpper;
        u32 interruptStatus;
        u32 interruptEnabled;
        u32 cmdStatus;
        u32 reserved0;
        u32 taskFileData;
        u32 signature;
        u32 sataStatus;
        u32 sataControll;
        u32 sataActive;
        u32 commandIssue;
        u32 sataNotification;
        u32 fisSwitchControl;
        u32 reserved1[11];
        u32 vendor[4];
    };

    struct HBAMmeory{
        u32 hostCapability;
        u32 globalHostControl;
        u32 interruptStatus;
        u32 portsImplemented;
        u32 version;
        u32 cccContorl;
        u32 cccPorts;
        u32 enclosureManagmentLocation;
        u32 enclosureManagmentControl;
        u32 hostCapabilitiesExtended;
        u32 biosHandoffCtrSts;
        u8 reserved[0x74];
        u8 vendor[0x60];
        HBAPort ports[1];
    };

    class AHCIDriver{
        HBAMmeory *ABAR;
        PCI::DeviceHeader *PCIDeviceAddress;
    
    public:
        AHCIDriver(PCI::DeviceHeader *pciBaseAddress);
        ~AHCIDriver();
    };
};

#endif /* ahci_hpp */
