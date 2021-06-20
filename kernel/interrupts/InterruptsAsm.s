.text

.global MachineCheckINT
.extern MachineCheckINT_Handler
.type MachineCheckINT,@function
    
.global VirtualizationINT
.extern VirtualizationINT_Handler
.type VirtualizationINT,@function
    
.global GeneralProtectionINT
.extern GeneralProtectionINT_Handler
.type GeneralProtectionINT,@function
    
.global DeviceNotAvailableINT
.extern DeviceNotAvailableINT_Handler
.type DeviceNotAvailableINT,@function
    
.global BoundRangeINT
.extern BoundRangeINT_Handler
.type BoundRangeINT,@function
    
.global PitINT
.extern PitINT_Handler
.type PitINT,@function
    
.global InvalidOpcodeINT
.extern InvalidOpcodeINT_Handler
.type InvalidOpcodeINT,@function
    
.global AlignmentCheckINT
.extern AlignmentCheckINT_Handler
.type AlignmentCheckINT,@function
    
.global DebugINT
.extern DebugINT_Handler
.type DebugINT,@function
    
.global StackSegmentINT
.extern StackSegmentINT_Handler
.type StackSegmentINT,@function
    
.global SegmentNotPresentINT
.extern SegmentNotPresentINT_Handler
.type SegmentNotPresentINT,@function
    
.global InavlidTSSINT
.extern InavlidTSSINT_Handler
.type InavlidTSSINT,@function
    
.global BreakpointINT
.extern BreakpointINT_Handler
.type BreakpointINT,@function
    
.global DoubleFaultINT
.extern DoubleFaultINT_Handler
.type DoubleFaultINT,@function
    
.global SIMDINT
.extern SIMDINT_Handler
.type SIMDINT,@function
    
.global EmptyIqrINT
.extern EmptyIqrINT_Handler
.type EmptyIqrINT,@function
    
.global OverflowINT
.extern OverflowINT_Handler
.type OverflowINT,@function
    
.global ERROR
.extern ERROR_Handler
.type ERROR,@function
    
.global DevisionByZeroINT
.extern DevisionByZeroINT_Handler
.type DevisionByZeroINT,@function
    
.global PageFaultINT
.extern PageFaultINT_Handler
.type PageFaultINT,@function
    
MachineCheckINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call MachineCheckINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

VirtualizationINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call VirtualizationINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

GeneralProtectionINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call GeneralProtectionINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

DeviceNotAvailableINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call DeviceNotAvailableINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

BoundRangeINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call BoundRangeINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

PitINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call PitINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

InvalidOpcodeINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call InvalidOpcodeINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

AlignmentCheckINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call AlignmentCheckINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

DebugINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call DebugINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

StackSegmentINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call StackSegmentINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

SegmentNotPresentINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call SegmentNotPresentINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

InavlidTSSINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call InavlidTSSINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

BreakpointINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call BreakpointINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

DoubleFaultINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call DoubleFaultINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

SIMDINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call SIMDINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

EmptyIqrINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call EmptyIqrINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

OverflowINT:

    sub $8, %rsp
    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    xorq %rsi, %rsi
    not %rsi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call OverflowINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    addq $8, %rsp
    
    iretq

DevisionByZeroINT:

    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    movq 0(%rsp), %rsi
    andl $0xFFFFFFFF, %esi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call DevisionByZeroINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    iretq

PageFaultINT:

    push %rbp
    mov %rsp, %rbp

    push %rax
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx

    lea    0(%rsp), %rdi
    movq 0(%rsp), %rsi
    andl $0xFFFFFFFF, %esi

    sub $0x100, %rsp
    movaps %xmm15, 0xf0(%rsp)
    movaps %xmm14, 0xe0(%rsp)
    movaps %xmm13, 0xd0(%rsp)
    movaps %xmm12, 0xc0(%rsp)
    movaps %xmm11, 0xb0(%rsp)
    movaps %xmm10, 0xa0(%rsp)
    movaps %xmm9, 0x90(%rsp)
    movaps %xmm8, 0x80(%rsp)
    movaps %xmm7, 0x70(%rsp)
    movaps %xmm6, 0x60(%rsp)
    movaps %xmm5, 0x50(%rsp)
    movaps %xmm4, 0x40(%rsp)
    movaps %xmm3, 0x30(%rsp)
    movaps %xmm2, 0x20(%rsp)
    movaps %xmm1, 0x10(%rsp)
    movaps %xmm0, (%rsp)
    cld

    call PageFaultINT_Handler

    movaps (%rsp),%xmm0
    movaps 0x10(%rsp),%xmm1
    movaps 0x20(%rsp),%xmm2
    movaps 0x30(%rsp),%xmm3
    movaps 0x40(%rsp),%xmm4
    movaps 0x50(%rsp),%xmm5
    movaps 0x60(%rsp),%xmm6
    movaps 0x70(%rsp),%xmm7
    movaps 0x80(%rsp),%xmm8
    movaps 0x90(%rsp),%xmm9
    movaps 0xa0(%rsp),%xmm10
    movaps 0xb0(%rsp),%xmm11
    movaps 0xc0(%rsp),%xmm12
    movaps 0xd0(%rsp),%xmm13
    movaps 0xe0(%rsp),%xmm14
    movaps 0xf0(%rsp),%xmm15
    add    $0x100,%rsp

    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15

    mov %rbp, %rsp
    pop %rbp
    iretq

