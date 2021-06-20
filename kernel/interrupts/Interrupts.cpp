#include "Interrupts.hpp"
#include <cerberus/printf.h>
#include "scheduling/pit/pit.hpp"
#include <memory/VMManager.hpp>
#include <memory/kmalloc.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

extern VMManager KernelVMM;


void PitINT_Handler                 (InterruptFrame_t *frame){
    PIT::Tick();
    PIC_EndMaster();
}

void SIMDINT_Handler                (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

void DebugINT_Handler               (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
}

void OverflowINT_Handler            (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

void EmptyIqrINT_Handler            (InterruptFrame_t *frame){
    return;
}

void BreakpointINT_Handler          (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
}

void BoundRangeINT_Handler          (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

void MachineCheckINT_Handler        (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

void InvalidOpcodeINT_Handler       (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

void VirtualizationINT_Handler      (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

void DevisionByZeroINT_Handler      (InterruptFrame_t *frame){
    frame->rax = UINT64_MAX;
    frame->rcx = 1UL;
}

void DeviceNotAvailableINT_Handler  (InterruptFrame_t *frame){
    cerbPrintf("Interrupt %s, from address %p", __FUNCTION__, frame->ip);
    ARCH::Go2Sleep();
}

void PageFaultINT_Handler           (InterruptFrame_t *frame, int error_code){
    u64 memoryRegion;

    __asm__ __volatile__(
        "movq %%cr2, %0\n"
        : "=r" (memoryRegion)
    );
   
    if (memoryRegion != 0x0 && memoryRegion >= MallocMainHeder.MallocBegin && memoryRegion < MallocMainHeder.MallocHead){
        auto page = PhisicalAllocator::Get2MB();
        KernelVMM.MapMemory2MB((void*)(memoryRegion - (memoryRegion & ((1<<21UL) - 1))), page);
    }
    else{
        cerbPrintf("Interrupt %s, from address %p, error code %#b, target address was %p", __FUNCTION__, frame->ip, frame->error_code & INT32_MAX, memoryRegion);
        ARCH::Go2Sleep();
    }
}

void InavlidTSSINT_Handler          (InterruptFrame_t *frame, int error_code){
    cerbPrintf("Interrupt %s, from address %p, error code %u", __FUNCTION__, frame->ip, error_code);
    ARCH::Go2Sleep();
}

void DoubleFaultINT_Handler         (InterruptFrame_t *frame, int error_code){
    cerbPrintf("Interrupt %s, from address %p, error code %u", __FUNCTION__, frame->ip, error_code);
    ARCH::Go2Sleep();
}

void StackSegmentINT_Handler        (InterruptFrame_t *frame, int error_code){
    cerbPrintf("Interrupt %s, from address %p, error code %u", __FUNCTION__, frame->ip, error_code);
    ARCH::Go2Sleep();
}

void AlignmentCheckINT_Handler      (InterruptFrame_t *frame, int error_code){
    cerbPrintf("Interrupt %s, from address %p, error code %u", __FUNCTION__, frame->ip, error_code);
    ARCH::Go2Sleep();
}

void GeneralProtectionINT_Handler   (InterruptFrame_t *frame, int error_code){
    cerbPrintf("Interrupt %s, from address %p, error code %u", __FUNCTION__, frame->ip, error_code);
    ARCH::Go2Sleep();
}

void SegmentNotPresentINT_Handler   (InterruptFrame_t *frame, int error_code){
    cerbPrintf("Interrupt %s, from address %p, error code %u", __FUNCTION__, frame->ip, error_code);
    ARCH::Go2Sleep();
}

void RemapPIC(){
    uint8_t a1, a2; 

    a1 = ARCH::inb(PIC1_DATA);
    ARCH::io_wait();
    a2 = ARCH::inb(PIC2_DATA);
    ARCH::io_wait();

    ARCH::outb(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND);
    ARCH::io_wait();
    ARCH::outb(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND);
    ARCH::io_wait();

    ARCH::outb(0x20, PIC1_DATA);
    ARCH::io_wait();
    ARCH::outb(0x28, PIC2_DATA);
    ARCH::io_wait();

    ARCH::outb(4, PIC1_DATA);
    ARCH::io_wait();
    ARCH::outb(2, PIC2_DATA);
    ARCH::io_wait();

    ARCH::outb(ICW4_8086, PIC1_DATA);
    ARCH::io_wait();
    ARCH::outb(ICW4_8086, PIC2_DATA);
    ARCH::io_wait();

    ARCH::outb(a1, PIC1_DATA);
    ARCH::io_wait();
    ARCH::outb(a2, PIC2_DATA);
}

#pragma GCC diagnostic pop
