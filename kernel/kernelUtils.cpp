#include <kernelUtils.hpp>
#include <pci.hpp>

IDTR KernelInfo::idtr;
GDTDescriptor KernelInfo::gdt;
PageTable *KernelInfo::PML4;
VMManager KernelVMM;
extern kernel_services_t *KS;

void KernelInfo::InitVMM(){
    PML4 = (PageTable*) PhisicalAllocator::Get();
    memset(PML4, 0, sizeof(PageTable));

    KernelVMM = VMManager(PML4);
    u64 fbBase = (u64)KS->frameBuffer.base_address;
    u64 fbSize = (u64)KS->frameBuffer.buffer_size + 0x1000;

    for (size_t i = fbBase; i < fbBase + fbSize; i += 0x1000){
        KernelVMM.MapMemory((void*)i, (void*)i);
    }

    for (size_t i = 0; i < PhisicalAllocator::GetTotalMemory() + 1; i += 0x1000){
        KernelVMM.MapMemory((void*)i, (void*)i);
    }

    __asm__ __volatile__ (
        "mov %0, %%cr3\n"
        :
        : "r" (PML4)
    );

    BasicRender::ClearScreen();
    Printf("KernelVMM ready. Used memeory: %llu KB\n", PhisicalAllocator::GetUsedMemory() / 1024);
}

void KernelInfo::InitGDT(){
    gdt.size = sizeof(GDT) - 1;
    gdt.address = (u64) &DefaultGDT;
    LoadGDT(&gdt);

    Printf("GDT ready\n");  
}

void KernelInfo::InitIDT(){
    idtr.limit = 0x0FFF;
    idtr.offset = (u64) PhisicalAllocator::Get();

    IDTDescriptorEntry *int_DevideByZero = (IDTDescriptorEntry*) (idtr.offset + 0x00 * sizeof(IDTDescriptorEntry));
    int_DevideByZero->setOffset((u64)DevideByZero_Handler);
    int_DevideByZero->selector = 0x08;
    int_DevideByZero->type_attr = IDT_TA_InterruptGate;

    IDTDescriptorEntry *int_Debug = (IDTDescriptorEntry*) (idtr.offset + 0x01 * sizeof(IDTDescriptorEntry));
    int_Debug->setOffset((u64)Debug_Handler);
    int_Debug->selector = 0x08;
    int_Debug->type_attr = IDT_TA_TrapGate;

    IDTDescriptorEntry *int_Breakpoint = (IDTDescriptorEntry*) (idtr.offset + 0x03 * sizeof(IDTDescriptorEntry));
    int_Breakpoint->setOffset((u64)Breakpoint_Handler);
    int_Breakpoint->selector = 0x08;
    int_Breakpoint->type_attr = IDT_TA_TrapGate;

    IDTDescriptorEntry *int_Overflow = (IDTDescriptorEntry*) (idtr.offset + 0x04 * sizeof(IDTDescriptorEntry));
    int_Overflow->setOffset((u64)Overflow_Handler);
    int_Overflow->selector = 0x08;
    int_Overflow->type_attr = IDT_TA_TrapGate;

    IDTDescriptorEntry *int_BoundRange = (IDTDescriptorEntry*) (idtr.offset + 0x05 * sizeof(IDTDescriptorEntry));
    int_BoundRange->setOffset((u64)BoundRange_Handler);
    int_BoundRange->selector = 0x08;
    int_BoundRange->type_attr = IDT_TA_InterruptGate;

    IDTDescriptorEntry *int_InvalidOpcode = (IDTDescriptorEntry*) (idtr.offset + 0x06 * sizeof(IDTDescriptorEntry));
    int_InvalidOpcode->setOffset((u64)InvalidOpcode_Handler);
    int_InvalidOpcode->selector = 0x08;
    int_InvalidOpcode->type_attr = IDT_TA_InterruptGate;

    IDTDescriptorEntry *int_DevideNotAvailable = (IDTDescriptorEntry*) (idtr.offset + 0x07 * sizeof(IDTDescriptorEntry));
    int_DevideNotAvailable->setOffset((u64)DeviceNotAvailable_Handler);
    int_DevideNotAvailable->selector = 0x08;
    int_DevideNotAvailable->type_attr = IDT_TA_InterruptGate;

    IDTDescriptorEntry *int_DoubleFault = (IDTDescriptorEntry*) (idtr.offset + 0x08 * sizeof(IDTDescriptorEntry));
    int_DoubleFault->setOffset((u64)DoubleFault_Handler);
    int_DoubleFault->selector = 0x08;
    int_DoubleFault->type_attr = IDT_TA_InterruptGate;

    IDTDescriptorEntry *int_PageFault = (IDTDescriptorEntry *) (idtr.offset + 0x0E * sizeof(IDTDescriptorEntry));
    int_PageFault->setOffset((u64)PageFault_Handler);
    int_PageFault->selector = 0x08;
    int_PageFault->type_attr = IDT_TA_InterruptGate;

    __asm__ __volatile__ (
        "lidt %0\n"
        :
        : "m" (idtr)
    );

    Printf("IDT ready\n");

}

void KernelInfo::InitACPI(){

    ACPI::SDTHeader *xsdt = (ACPI::SDTHeader *) (((ACPI::RSDP2*)KS->rsdp)->XSDTAddress);
    ACPI::MCFGHeader *mcfg = (ACPI::MCFGHeader*) ACPI::FindTable(xsdt, (char*) "MCFG");
    
    PCI::EnumeratePCI(mcfg);
}

void KernelInfo::Init(){
    Printf("Wait for VMM initialization\n");
    PhisicalAllocator::Init();
    Printf("PhisicalAllocator initialized\n");
    InitVMM();
    InitGDT();
    InitIDT();
    InitACPI();
  
}
