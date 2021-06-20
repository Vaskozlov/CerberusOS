import sys

OUTPUTFILE = "./interrupts/InterruptsAsm.s"

FIRST_PATH_OF_INTERRUPTS_WITHOUT_ERROR_CODE = """
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
"""

SECOND_PATH_OF_INTERRUPTS_WITHOUT_ERROR_CODE = """
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
"""

FIRST_PATH_OF_INTERRUPTS_WITH_ERROR_CODE = """
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
"""

SECOND_PATH_OF_INTERRUPTS_WITH_ERROR_CODE = """
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
"""

InterruptsWithoutErrors = set()
for elem in sys.argv[1:]:
    InterruptsWithoutErrors.add(elem)

RESULT = ".text\n"

for elem in InterruptsWithoutErrors:
    RESULT += f"""
.global {elem}
.extern {elem}_Handler
.type {elem},@function
    """

FIRST_PART = FIRST_PATH_OF_INTERRUPTS_WITHOUT_ERROR_CODE
SECOND_PART = SECOND_PATH_OF_INTERRUPTS_WITHOUT_ERROR_CODE

for elem in InterruptsWithoutErrors:
    if (elem == "ERROR"):
        FIRST_PART = FIRST_PATH_OF_INTERRUPTS_WITH_ERROR_CODE
        SECOND_PART = SECOND_PATH_OF_INTERRUPTS_WITH_ERROR_CODE
        continue

    RESULT += f"\n{elem}:\n{FIRST_PART}\n    call {elem}_Handler\n{SECOND_PART}"

with open(OUTPUTFILE, mode="w") as fin:
    print(RESULT, file=fin)
