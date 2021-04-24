#include "kernelUtils.hpp"
#include "printf/Printf.h"
#include "hardware/acpi.hpp"
#include "hardware/pci.hpp"
#include "scheduling/pit/pit.hpp"
#include "render/basicFrameManager.hpp"
#include "interrupts/Interrupts.h"
#include "memory/kmalloc.h"

IDTR KernelInfo::idtr;
GDTDescriptor KernelInfo::gdt;
PageTable *KernelInfo::PML4;
VMManager KernelVMM;
extern kernel_services_t *KS;

__attribute__((aligned(0x1000))) u8 IDTBuffer[0x1000];

void KernelInfo::InitVMM(){
    PML4 = (PageTable*) PhisicalAllocator::Get4KB();
    memset(PML4, 0, sizeof(PageTable));

    KernelVMM = VMManager(PML4);
    u64 fbBase = (u64)KS->frameBuffer.base_address;
    u64 fbSize = (u64)KS->frameBuffer.buffer_size + (1 << 21UL);
    fbBase -= (fbBase & ((1 << 21UL) - 1));

    u64 totalMemory = PhisicalAllocator::GetTotalMemory();

    for (u64 i = 0x1000; i < 512 * 0x1000; i += 0x1000)
        KernelVMM.MapMemory4KB((void*)i, (void*)i);

    for (u64 i = (1<<21UL); i < (1<<30UL) && i < totalMemory; i += (1<<21UL))
        KernelVMM.MapMemory2MB((void*)i, (void*)i);

    for (size_t i = 1; i < align(PhisicalAllocator::GetTotalMemory(), 30); i += (1 << 30UL))
        KernelVMM.MapMemory1GB((void*)i, (void*)i);

    for (size_t i = fbBase; i < fbBase + fbSize; i += (1 << 21UL))
        KernelVMM.MapMemory2MB((void*)i, (void*)i);
   
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

    kprintf("GDT ready %p\n", &DefaultGDT);  
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
    
    SetUpIDTEntry(  (void*) DevideByZero_Handler,       0x00, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) Debug_Handler,              0x01, 0x08, IDT_TA_TrapGate     );
    SetUpIDTEntry(  (void*) Breakpoint_Handler,         0x03, 0x08, IDT_TA_TrapGate     );
    SetUpIDTEntry(  (void*) Overflow_Handler,           0x04, 0x08, IDT_TA_TrapGate     );
    SetUpIDTEntry(  (void*) BoundRange_Handler,         0x05, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) InvalidOpcode_Handler,      0x06, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) DeviceNotAvailable_Handler, 0x07, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) DoubleFault_Handler,        0x08, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) SegmentNotPresent_Handler,  0x0B, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) GeneralProtection_Handler,  0x0D, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) PageFault_Handler,          0x0E, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) Pit_Handler,                0x20, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) EmptyIQR_Handler,           0x21, 0x08, IDT_TA_InterruptGate);
    SetUpIDTEntry(  (void*) EmptyIQR_Handler,           0x2C, 0x08, IDT_TA_InterruptGate);

    __asm__ __volatile__ ("lidt %0" : : "m" (idtr));

    RemapPIC();
    kprintf("IDT and PIC ready\n");
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
    
    kprintf("Wait for VMM initialization\n");
    PhisicalAllocator::SetUp();

    InitVMM();
    InitKMalloc();
    //InitACPI();

    BasicRender::ClearScreen();

    kprintf( 
        "KernelInfo ready in \xff\xff\0\xff%lu\xff\xff\xff\xff MS."
        "Available memeory: \xff\xff\0\xff%llu MB\xff\xff\xff\xff, " 
        "Cerberus majore version \xff\xff\0\xff%u\xff\xff\xff\xff, "
        "Cerberus minore version \xff\xff\0\xff%u\xff\xff\xff\xff, "
        "compiled with \xff\xff\0\xff%s\xff\xff\xff\xff\n",
        PIT::GetTimeSicneBoot(),
        PhisicalAllocator::GetAvailableMemory() / 1024 / 1024,
        CERBERUS_MAJORE_VERSION,
        CERBERUS_MINORE_VERSION,
        COMPILER_NAME
    );
}
