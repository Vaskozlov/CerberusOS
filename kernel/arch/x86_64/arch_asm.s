.text

.global EnableSSEIN
.global CPU_ID
.global memset
.global memset8
.global memset16
.global memset32
.global memset64

.global memcpy8
.global memcpy16
.global memcpy32
.global memcpy64

.type EnableSSEIN,@function
.type CPU_ID,@function
.type memset,@function
.type memset8,@function
.type memset16,@function
.type memset32,@function
.type memset64,@function

.type memcpy8,@function
.type memcpy16,@function
.type memcpy32,@function
.type memcpy64,@function

EnableSSEIN:
    movq %cr0, %rax
    andw $0xFFFB, %ax
    orw $2, %ax
    movq %rax, %cr0

    movq %cr4, %rax
    orw $0x600, %ax
    movq %rax, %cr4
    retq
CPU_ID:
    movq %rsi, %rax
    movq %rdx, %rcx

    cpuid

    movq %rax, 0(%rdi)
    movq %rcx, 8(%rdi)
    movq %rdx, 16(%rdi)

    movq %rdi, %rax
    
    retq

memset8:
    movb %sil, %al
    movq %rdx, %rcx
    rep stosb
    retq

memset16:
    movw %si, %ax
    movq %rdx, %rcx
    rep stosw
    retq

memset32:
    movl %esi, %eax
    movq %rdx, %rcx
    rep stosl
    retq

memset64:
    movq %rsi, %rax
    movq %rdx, %rcx
    rep stosq
    retq

memset:
    cmp $2, %rdx
    jz .memset2
    
    movb %sil, %al
    movq %rdx, %rcx
    rep stosb
    retq
    
.memset2:
    shr $2, %rdx

    movl %esi, %eax
    movq %rdx, %rcx
    rep stosl
    retq

memcpy8:
    movq %rdx, %rcx
    rep movsb
    retq

memcpy16:
    movq %rdx, %rcx
    rep movsw
    retq

memcpy32:
    movq %rdx, %rcx
    rep movsl
    retq

memcpy64:
    movq %rdx, %rcx
    rep movsq
    retq