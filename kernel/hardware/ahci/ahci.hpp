#pragma once

#include <kernel.h>
#include <hardware/pci.hpp>

namespace AHCI {

    #define ATA_DEV_BUSY 0x80
    #define ATA_DEV_DRQ 0x08
    #define ATA_CMD_READ_DMA_EX 0x25

    #define HBA_PxIS_TFES (1 << 30)

    enum PortType {
        None    = 0,
        SATA    = 1,
        SEMB    = 2,
        PM      = 3,
        SATAPI  = 4,
    };

    enum FIS_TYPE{
        FIS_TYPE_REG_H2D    = 0x27,
        FIS_TYPE_REG_D2H    = 0x34,
        FIS_TYPE_DMA_ACT    = 0x39,
        FIS_TYPE_DMA_SETUP  = 0x41,
        FIS_TYPE_DATA       = 0x46,
        FIS_TYPE_BIST       = 0x58,
        FIS_TYPE_PIO_SETUP  = 0x5F,
        FIS_TYPE_DEV_BITS   = 0xA1,
    };

    struct HBAPort{
        u32 commandListBase;
        u32 commandListBaseUpper;
        u32 fisBaseAddress;
        u32 fisBaseAddressUpper;
        u32 interruptStatus;
        u32 interruptEnable;
        u32 cmdSts;
        u32 rsv0;
        u32 taskFileData;
        u32 signature;
        u32 sataStatus;
        u32 sataControl;
        u32 sataError;
        u32 sataActive;
        u32 commandIssue;
        u32 sataNotification;
        u32 fisSwitchControl;
        u32 rsv1[11];
        u32 vendor[4];
    };

    struct HBAMemory{
        u32 hostCapability;
        u32 globalHostControl;
        u32 interruptStatus;
        u32 portsImplemented;
        u32 version;
        u32 cccControl;
        u32 cccPorts;
        u32 enclosureManagementLocation;
        u32 enclosureManagementControl;
        u32 hostCapabilitiesExtended;
        u32 biosHandoffCtrlSts;
        u8 rsv0[0x74];
        u8 vendor[0x60];
        HBAPort ports[1];
    };

    struct HBACommandHeader {
        u8 commandFISLength:5;
        u8 atapi:1;
        u8 write:1;
        u8 prefetchable:1;

        u8 reset:1;
        u8 bist:1;
        u8 clearBusy:1;
        u8 rsv0:1;
        u8 portMultiplier:4;

        u16 prdtLength;
        u32 prdbCount;
        u32 commandTableBaseAddress;
        u32 commandTableBaseAddressUpper;
        u32 rsv1[4];
    };

    struct HBAPRDTEntry{
        u32 dataBaseAddress;
        u32 dataBaseAddressUpper;
        u32 rsv0;

        u32 byteCount:22;
        u32 rsv1:9;
        u32 interruptOnCompletion:1;
    };

    struct HBACommandTable{
        u8 commandFIS[64];
        u8 atapiCommand[16];
        u8 rsv[48];
        HBAPRDTEntry prdtEntry[];
    };

    struct FIS_REG_H2D {
        u8 fisType;

        u8 portMultiplier:4;
        u8 rsv0:3;
        u8 commandControl:1;

        u8 command;
        u8 featureLow;

        u8 lba0;
        u8 lba1;
        u8 lba2;
        u8 deviceRegister;

        u8 lba3;
        u8 lba4;
        u8 lba5;
        u8 featureHigh;

        u8 countLow;
        u8 countHigh;
        u8 isoCommandCompletion;
        u8 control;

        u8 rsv1[4];
    };

    struct Port {
        PortType portType;
        u8* buffer;
        u8 portNumber;
        HBAPort* hbaPort;

        void Configure();
        void StartCMD();
        void StopCMD();
        bool Read(u64 sector, u32 sectorCount, void* buffer);
    };

    class AHCIDriver{
        public:
        AHCIDriver(PCI::DeviceHeader* pciBaseAddress);
        ~AHCIDriver();
        PCI::DeviceHeader* PCIBaseAddress;
        HBAMemory* ABAR;
        void ProbePorts();
        Port* ports[32];
        u8 portCount;
    };
}