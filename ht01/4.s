	.file	"4.c"
	.text
	.section	.rodata
	.align 4
.LC0:
	.string	"invalid argument signature: %c\n"
	.text
	.type	stack_size, @function
stack_size:
.LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$0, -12(%ebp)
	jmp	.L2
.L10:
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	cmpl	$105, %eax
	je	.L4
	cmpl	$105, %eax
	jg	.L5
	cmpl	$100, %eax
	je	.L6
	jmp	.L3
.L5:
	cmpl	$115, %eax
	je	.L7
	cmpl	$118, %eax
	je	.L12
	jmp	.L3
.L4:
	movl	-12(%ebp), %eax
	addl	$4, %eax
	movl	%eax, -12(%ebp)
	jmp	.L9
.L7:
	movl	-12(%ebp), %eax
	addl	$4, %eax
	movl	%eax, -12(%ebp)
	jmp	.L9
.L6:
	movl	-12(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -12(%ebp)
	jmp	.L9
.L3:
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %edx
	movl	stderr@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	%edx
	leal	.LC0@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	%eax
	call	fprintf@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1
	call	exit@PLT
.L12:
	nop
.L9:
	addl	$1, 8(%ebp)
.L2:
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L10
	movl	-12(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	stack_size, .-stack_size
	.section	.rodata
.LC1:
	.string	"invalid argument sig: %c\n"
	.text
	.globl	stack_fill
	.type	stack_fill, @function
stack_fill:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$0, -12(%ebp)
	movl	$0, -20(%ebp)
	jmp	.L14
.L22:
	movl	12(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	cmpl	$105, %eax
	je	.L16
	cmpl	$105, %eax
	jg	.L17
	cmpl	$100, %eax
	je	.L18
	jmp	.L15
.L17:
	cmpl	$115, %eax
	je	.L19
	cmpl	$118, %eax
	je	.L20
	jmp	.L15
.L16:
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	16(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	$0
	pushl	$0
	pushl	%eax
	call	strtol@PLT
	addl	$16, %esp
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	$4, -16(%ebp)
	jmp	.L21
.L19:
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	16(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	$4, -16(%ebp)
	jmp	.L21
.L18:
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	16(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	$0
	pushl	%eax
	call	strtod@PLT
	addl	$16, %esp
	movl	8(%ebp), %eax
	fstpl	(%eax)
	movl	$8, -16(%ebp)
	jmp	.L21
.L20:
	movl	$0, -16(%ebp)
	jmp	.L21
.L15:
	movl	12(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %edx
	movl	stderr@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	%edx
	leal	.LC1@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	%eax
	call	fprintf@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1
	call	exit@PLT
.L21:
	movl	-16(%ebp), %eax
	andl	$-4, %eax
	addl	%eax, 8(%ebp)
	movl	-16(%ebp), %eax
	shrl	$2, %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -12(%ebp)
	addl	$1, 12(%ebp)
	addl	$1, -20(%ebp)
.L14:
	movl	12(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L22
	movl	-12(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	stack_fill, .-stack_fill
	.section	.rodata
.LC2:
	.string	"%d\n"
.LC3:
	.string	"%.10g\n"
	.align 4
.LC4:
	.string	"invalid return value signature: %c\n"
	.text
	.globl	print_result
	.type	print_result, @function
print_result:
.LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	20(%ebp), %ecx
	movl	12(%ebp), %edx
	movl	%edx, -16(%ebp)
	movl	16(%ebp), %edx
	movl	%edx, -12(%ebp)
	movl	%ecx, %edx
	movb	%dl, -20(%ebp)
	movsbl	-20(%ebp), %edx
	cmpl	$105, %edx
	je	.L26
	cmpl	$105, %edx
	jg	.L27
	cmpl	$100, %edx
	je	.L28
	jmp	.L25
.L27:
	cmpl	$115, %edx
	je	.L29
	cmpl	$118, %edx
	je	.L32
	jmp	.L25
.L26:
	subl	$8, %esp
	pushl	8(%ebp)
	leal	.LC2@GOTOFF(%eax), %edx
	pushl	%edx
	movl	%eax, %ebx
	call	printf@PLT
	addl	$16, %esp
	jmp	.L31
.L29:
	movl	8(%ebp), %edx
	subl	$12, %esp
	pushl	%edx
	movl	%eax, %ebx
	call	puts@PLT
	addl	$16, %esp
	jmp	.L31
.L28:
	subl	$4, %esp
	pushl	-12(%ebp)
	pushl	-16(%ebp)
	leal	.LC3@GOTOFF(%eax), %edx
	pushl	%edx
	movl	%eax, %ebx
	call	printf@PLT
	addl	$16, %esp
	jmp	.L31
.L25:
	movsbl	-20(%ebp), %ecx
	movl	stderr@GOT(%eax), %edx
	movl	(%edx), %edx
	subl	$4, %esp
	pushl	%ecx
	leal	.LC4@GOTOFF(%eax), %ecx
	pushl	%ecx
	pushl	%edx
	movl	%eax, %ebx
	call	fprintf@PLT
	addl	$16, %esp
	jmp	.L31
.L32:
	nop
.L31:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	print_result, .-print_result
	.section	.rodata
	.align 4
.LC5:
	.string	"at least 3 arguments expected: <libname> <funcname> <dl_sign> [arguments...]\n"
	.align 4
.LC6:
	.string	"can not allocate stack: %d bytes\n"
.LC7:
	.string	"dlopen(%s): %s\n"
.LC8:
	.string	"dlsym(%s): %s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x78,0x6
	.cfi_escape 0x10,0x3,0x2,0x75,0x7c
	subl	$80, %esp
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	%ecx, %eax
	movl	4(%eax), %edx
	movl	%edx, -76(%ebp)
	movl	%gs:20, %ecx
	movl	%ecx, -12(%ebp)
	xorl	%ecx, %ecx
	cmpl	$3, (%eax)
	jg	.L34
	movl	stderr@GOT(%ebx), %eax
	movl	(%eax), %eax
	pushl	%eax
	pushl	$77
	pushl	$1
	leal	.LC5@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fwrite@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1
	call	exit@PLT
.L34:
	movl	-76(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -48(%ebp)
	movl	-76(%ebp), %eax
	movl	8(%eax), %eax
	movl	%eax, -44(%ebp)
	movl	-76(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, -40(%ebp)
	movl	$0, -60(%ebp)
	movl	$0, -56(%ebp)
	movl	-40(%ebp), %eax
	addl	$1, %eax
	subl	$12, %esp
	pushl	%eax
	call	stack_size
	addl	$16, %esp
	movl	%eax, -36(%ebp)
	cmpl	$0, -36(%ebp)
	je	.L35
	movl	-36(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	malloc@PLT
	addl	$16, %esp
	movl	%eax, -60(%ebp)
	cmpl	$0, -60(%ebp)
	jne	.L36
	movl	stderr@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	-36(%ebp)
	leal	.LC6@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	%eax
	call	fprintf@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1
	call	exit@PLT
.L36:
	movl	-76(%ebp), %eax
	leal	16(%eax), %edx
	movl	-40(%ebp), %eax
	addl	$1, %eax
	subl	$4, %esp
	pushl	%edx
	pushl	%eax
	pushl	-60(%ebp)
	call	stack_fill
	addl	$16, %esp
	movl	%eax, -56(%ebp)
.L35:
	subl	$8, %esp
	pushl	$1
	pushl	-48(%ebp)
	call	dlopen@PLT
	addl	$16, %esp
	movl	%eax, -32(%ebp)
	cmpl	$0, -32(%ebp)
	jne	.L37
	call	dlerror@PLT
	movl	%eax, %edx
	movl	stderr@GOT(%ebx), %eax
	movl	(%eax), %eax
	pushl	%edx
	pushl	-48(%ebp)
	leal	.LC7@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	%eax
	call	fprintf@PLT
	addl	$16, %esp
	movl	$1, %eax
	jmp	.L44
.L37:
	subl	$8, %esp
	pushl	-44(%ebp)
	pushl	-32(%ebp)
	call	dlsym@PLT
	addl	$16, %esp
	movl	%eax, -28(%ebp)
	cmpl	$0, -28(%ebp)
	jne	.L39
	call	dlerror@PLT
	movl	%eax, %edx
	movl	stderr@GOT(%ebx), %eax
	movl	(%eax), %eax
	pushl	%edx
	pushl	-44(%ebp)
	leal	.LC8@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	%eax
	call	fprintf@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	-32(%ebp)
	call	dlclose@PLT
	addl	$16, %esp
	movl	$1, %eax
	jmp	.L44
.L39:
	cmpl	$0, -36(%ebp)
	je	.L40
	movl	-56(%ebp), %eax
	addl	$1073741823, %eax
	leal	0(,%eax,4), %edx
	movl	-60(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -52(%ebp)
	jmp	.L41
.L42:
	movl	-52(%ebp), %eax
#APP
# 157 "4.c" 1
	push (%eax)

# 0 "" 2
#NO_APP
	subl	$4, -52(%ebp)
.L41:
	movl	-52(%ebp), %eax
	cmpl	-60(%ebp), %eax
	jnb	.L42
.L40:
	movl	-28(%ebp), %eax
	call	*%eax
#APP
# 164 "4.c" 1
	mov %eax, -64(%ebp)

# 0 "" 2
# 167 "4.c" 1
	fstpl -24(%ebp)

# 0 "" 2
#NO_APP
	cmpl	$0, -36(%ebp)
	je	.L43
#APP
# 170 "4.c" 1
	add -36(%ebp), %esp

# 0 "" 2
#NO_APP
.L43:
	movl	-40(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %edx
	fldl	-24(%ebp)
	movl	-64(%ebp), %eax
	pushl	%edx
	leal	-8(%esp), %esp
	fstpl	(%esp)
	pushl	%eax
	call	print_result
	addl	$16, %esp
	subl	$12, %esp
	pushl	-32(%ebp)
	call	dlclose@PLT
	addl	$16, %esp
	movl	$0, %eax
.L44:
	movl	-12(%ebp), %ecx
	xorl	%gs:20, %ecx
	je	.L45
	call	__stack_chk_fail_local
.L45:
	leal	-8(%ebp), %esp
	popl	%ecx
	.cfi_restore 1
	.cfi_def_cfa 1, 0
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB9:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE9:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB10:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE10:
	.hidden	__stack_chk_fail_local
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
