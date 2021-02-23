	.file	"5.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d\n"
	.text
	.p2align 4,,15
	.globl	handler
	.type	handler, @function
handler:
.LFB56:
	.cfi_startproc
	cmpl	$15, %edi
	je	.L7
	cmpl	$2, %edi
	je	.L9
	rep ret
	.p2align 4,,10
	.p2align 3
.L9:
	cmpl	$3, counter.4320(%rip)
	je	.L7
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	last_prime(%rip), %edx
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	addl	$1, counter.4320(%rip)
	movq	stdout(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	fflush@PLT
	.p2align 4,,10
	.p2align 3
.L7:
	movl	$1, to_terminate(%rip)
	ret
	.cfi_endproc
.LFE56:
	.size	handler, .-handler
	.p2align 4,,15
	.globl	is_prime
	.type	is_prime, @function
is_prime:
.LFB57:
	.cfi_startproc
	xorl	%eax, %eax
	cmpl	$1, %edi
	jbe	.L10
	cmpl	$3, %edi
	jbe	.L15
	testb	$1, %dil
	je	.L10
	movl	$2, %ecx
	jmp	.L12
	.p2align 4,,10
	.p2align 3
.L13:
	xorl	%edx, %edx
	movl	%edi, %eax
	divl	%ecx
	testl	%edx, %edx
	je	.L17
.L12:
	addl	$1, %ecx
	movl	%ecx, %eax
	imull	%ecx, %eax
	cmpl	%eax, %edi
	jnb	.L13
.L15:
	movl	$1, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L17:
	xorl	%eax, %eax
.L10:
	rep ret
	.cfi_endproc
.LFE57:
	.size	is_prime, .-is_prime
	.section	.rodata.str1.1
.LC1:
	.string	"-1"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB58:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	xorl	%edx, %edx
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rsi, %rbp
	xorl	%r13d, %r13d
	leaq	handler(%rip), %r12
	subq	$328, %rsp
	.cfi_def_cfa_offset 368
	movq	8(%rsi), %rdi
	xorl	%esi, %esi
	movq	%fs:40, %rax
	movq	%rax, 312(%rsp)
	xorl	%eax, %eax
	call	strtol@PLT
	movq	16(%rbp), %rdi
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%rax, %rbx
	call	strtol@PLT
	leaq	8(%rsp), %rdi
	movq	%rax, %rbp
	movl	$18, %ecx
	movq	%r13, %rax
	xorl	%edx, %edx
	movq	%rsp, %rsi
	rep stosq
	movl	$2, %edi
	movq	%r12, (%rsp)
	movl	$268435456, 136(%rsp)
	call	sigaction@PLT
	leaq	168(%rsp), %rdi
	movq	%r13, %rax
	movl	$18, %ecx
	leaq	160(%rsp), %rsi
	xorl	%edx, %edx
	movq	%r12, 160(%rsp)
	rep stosq
	movl	$15, %edi
	movl	$268435456, 296(%rsp)
	call	sigaction@PLT
	call	getpid@PLT
	leaq	.LC0(%rip), %rsi
	movl	%eax, %edx
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movq	stdout(%rip), %rdi
	call	fflush@PLT
	cmpl	%ebp, %ebx
	jge	.L19
	movl	to_terminate(%rip), %eax
	testl	%eax, %eax
	jne	.L20
	movl	last_prime(%rip), %r9d
	leal	-1(%rbp), %edi
	xorl	%r8d, %r8d
	jmp	.L21
	.p2align 4,,10
	.p2align 3
.L22:
	cmpl	%ebx, %edi
	je	.L43
.L25:
	movl	to_terminate(%rip), %edx
	addl	$1, %ebx
	testl	%edx, %edx
	jne	.L44
.L21:
	cmpl	$1, %ebx
	jbe	.L22
	cmpl	$3, %ebx
	jbe	.L27
	testb	$1, %bl
	je	.L22
	movl	$2, %ecx
	jmp	.L23
	.p2align 4,,10
	.p2align 3
.L24:
	xorl	%edx, %edx
	movl	%ebx, %eax
	divl	%ecx
	testl	%edx, %edx
	je	.L22
.L23:
	addl	$1, %ecx
	movl	%ecx, %edx
	imull	%ecx, %edx
	cmpl	%ebx, %edx
	jbe	.L24
.L27:
	cmpl	%ebx, %edi
	movl	%ebx, %r9d
	movl	$1, %r8d
	jne	.L25
.L43:
	testb	%r8b, %r8b
	jne	.L45
.L19:
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
.L20:
	xorl	%eax, %eax
	movq	312(%rsp), %rsi
	xorq	%fs:40, %rsi
	jne	.L46
	addq	$328, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L44:
	.cfi_restore_state
	testb	%r8b, %r8b
	je	.L20
	movl	%r9d, last_prime(%rip)
	jmp	.L20
.L45:
	movl	%r9d, last_prime(%rip)
	jmp	.L19
.L46:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE58:
	.size	main, .-main
	.local	counter.4320
	.comm	counter.4320,4,4
	.globl	to_terminate
	.bss
	.align 4
	.type	to_terminate, @object
	.size	to_terminate, 4
to_terminate:
	.zero	4
	.local	last_prime
	.comm	last_prime,4,4
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
