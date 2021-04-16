.global LoadGDT
.text
.type LoadGDT,@function

LoadGDT:
    lgdt 0(%rdi)
    xor %rax, %rax
    mov $0x10, %ax
    mov %eax, %ds
    mov %eax, %es
    mov %eax, %fs
    mov %eax, %gs
    mov %eax, %ss
    pop %rdi
    mov $0x08, %eax
    push %rax
    push %rdi
lretq
