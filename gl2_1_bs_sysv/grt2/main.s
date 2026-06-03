

.section .text
.globl sys_write
.globl sys_read
.globl sys_open
.globl sys_close
.globl sys_lseek
.globl sys_mmap
.globl sys_munmap
.globl sys_mremap
.globl sys_exit

# syscall differs from callq
# syscall overwrites %rcx and %r11
# syscall 4th arg is %r10 not %rcx
# syscall %rax stores syscall id number

sys_write:
    movq %rcx, %r10
    movq $1, %rax
    syscall
    retq
sys_read:
    movq %rcx, %r10
    movq $0, %rax
    syscall
    retq
sys_open:
    movq %rcx, %r10
    movq $2, %rax
    syscall
    retq
sys_close:
    movq %rcx, %r10
    movq $3, %rax
    syscall
    retq
sys_lseek:
    movq %rcx, %r10
    movq $8, %rax
    syscall
    retq
sys_mmap:
    movq %rcx, %r10
    movq $9, %rax
    syscall
    retq
sys_munmap:
    movq %rcx, %r10
    movq $11, %rax
    syscall
    retq
sys_mremap:
    movq %rcx, %r10
    movq $25, %rax
    syscall
    retq
sys_exit:
    movq %rcx, %r10
    movq $60, %rax
    syscall
    retq

.globl _start

/*_start:
    xorq %rbp, %rbp
    movq (%rsp), %rdi
    leaq 8(%rsp), %rsi
    subq $8, %rsp
    callq main
    addq $8, %rsp
    movq %rax, %rdi
    movq $60, %rax
    syscall
*/

_start:
    xorq %rbp, %rbp
    popq %rdi
    movq %rsp, %rsi
    subq $8, %rsp
    callq main
    addq $8, %rsp
    movq %rax, %rdi
    movq $60, %rax
    syscall
