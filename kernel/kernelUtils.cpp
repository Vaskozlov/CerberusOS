#include <arch.hpp>
#include "kernelUtils.hpp"
#include "hardware/acpi.hpp"
#include "hardware/pci.hpp"
#include "scheduling/pit/pit.hpp"
#include "render/basicFrameManager.hpp"
#include "interrupts/Interrupts.hpp"
#include "memory/kmalloc.h"
#include <cerberus/printf.h>

IDTR KernelInfo::idtr;
GDTDescriptor KernelInfo::gdt;
PageTable *KernelInfo::PML4;
VMManager KernelVMM;

__attribute__((aligned(0x1000))) u8 IDTBuffer[0x1000];

void KernelInfo::InitVMM(){
    PML4 = (PageTable*) PhisicalAllocator::Get4KB();
    ARCH::memset64(PML4, 0UL, sizeof(PageTable) / sizeof(u64));

    KernelVMM = VMManager(PML4);
    u64 totalMemory = PhisicalAllocator::GetTotalMemory();

    for (u64 i = 0x1000; i < 512 * 0x1000; i += 0x1000)
        KernelVMM.MapMemory4KB((void*)i, (void*)i);

    for (u64 i = (1<<21UL); i < (1<<30UL) && i < totalMemory; i += (1<<21UL))
        KernelVMM.MapMemory2MB((void*)i, (void*)i);

    for (size_t i = (1<<30UL); i < cerb::align(PhisicalAllocator::GetTotalMemory(), 30); i += (1 << 30UL))
        KernelVMM.MapMemory1GB((void*)i, (void*)i);

    __asm__ __volatile__ (
        "mov %0, %%cr3\n"
        :
        : "r" (PML4)
    );
}

void KernelInfo::InitGDT(){
    gdt.size = sizeof(GDT) - 1;
    gdt.address = (u64) &DefaultGDT;
    LoadGDT(&gdt);
    cerbPrintf("GDT ready %p\n", &DefaultGDT);  
}

void KernelInfo::SetUpIDTEntry(void *handler, u16 position, u8 selector, u8 type_attr){
    IDTDescriptorEntry *int_Handler = (IDTDescriptorEntry *)(idtr.offset + position * sizeof(IDTDescriptorEntry));
    int_Handler->selector = selector;
    int_Handler->setOffset((u64)handler);
    int_Handler->type_attr = type_attr;
}

void KernelInfo::InitIDT(){
    idtr.limit = 0x0FFF;
    idtr.offset = (u64) IDTBuffer;
    
    SetUpIDTEntry(  (void*) DevisionByZeroINT,     0x00, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) DebugINT,              0x01, 0x08, IDT_TA_TrapGate     );
    SetUpIDTEntry(  (void*) BreakpointINT,         0x03, 0x08, IDT_TA_TrapGate     );
    SetUpIDTEntry(  (void*) OverflowINT,           0x04, 0x08, IDT_TA_TrapGate     );
    SetUpIDTEntry(  (void*) BoundRangeINT,         0x05, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) InvalidOpcodeINT,      0x06, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) DeviceNotAvailableINT, 0x07, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) DoubleFaultINT,        0x08, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) InavlidTSSINT,         0x0A, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) SegmentNotPresentINT,  0x0B, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) SegmentNotPresentINT,  0x0C, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) GeneralProtectionINT,  0x0D, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) PageFaultINT,          0x0E, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) AlignmentCheckINT,     0x11, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) MachineCheckINT,       0x12, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) SIMDINT,               0x13, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) VirtualizationINT,     0x14, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) PitINT,                0x20, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) EmptyIqrINT,           0x21, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) EmptyIqrINT,           0x2C, 0x08, IDT_TA_InterruptGate);

    __asm__ __volatile__ ("lidt %0" : : "m" (idtr));

    RemapPIC();
    cerbPrintString("IDT and PIC ready\n");
}

void KernelInfo::InitACPI(){
    ACPI::SDTHeader *xsdt = (ACPI::SDTHeader *) (((ACPI::RSDP2*)KS->rsdp)->XSDTAddress);
    ACPI::MCFGHeader *mcfg = (ACPI::MCFGHeader*) ACPI::FindTable(xsdt, (char*) "MCFG");
    
    PCI::EnumeratePCI(mcfg);
}

void KernelInfo::Init(){
    InitGDT();
    InitIDT();
    
    ARCH::outb(0x08, PIC1_DATA);
    ARCH::outb(0x70, PIC2_DATA);

    __asm__ __volatile__ ("sti");

    PIT::SetFrequency(1000);
    
    cerbPrintString("Wait for VMM initialization\n");
    PhisicalAllocator::SetUp();

    InitVMM();
    InitKMalloc();

    BasicRender::ClearScreen();
    InitACPI();

    cerbPrintf( 
        "KernelInfo ready in %lu MS."
        "Available memeory: \xff\xff\0\xff%llu MB\xff\xff\xff\xff, " 
        "Cerberus major version \xff\xff\0\xff%u\xff\xff\xff\xff, "
        "Cerberus minor version \xff\xff\0\xff%u\xff\xff\xff\xff, "
        "compiled with \xff\xff\0\xff%s\xff\xff\xff\xff\n",
        PIT::GetTimeSicneBoot(),
        PhisicalAllocator::GetAvailableMemory() / 1024 / 1024,
        CERBERUS_MAJOR_VERSION,
        CERBERUS_MINOR_VERSION,
        COMPILER_NAME
    );
}
