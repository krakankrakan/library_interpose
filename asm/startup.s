	.file	"startup.c"
	.text
	.comm	trampoline_original_addr,8192,32
	.section	.rodata
.LC0:
	.string	"\n"
	.text
	.globl	orig_puts
	.type	orig_puts, @function
orig_puts:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -5(%rbp)
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movb	%al, -5(%rbp)
	addl	$1, -4(%rbp)
.L2:
	cmpb	$0, -5(%rbp)
	jne	.L3
	movl	$1, %eax
	movl	$1, %edi
	movq	-24(%rbp), %rsi
	movl	-4(%rbp), %edx
#APP
# 38 "startup.c" 1
	syscall
# 0 "" 2
#NO_APP
	movl	$1, %eax
	movl	$1, %edi
	leaq	.LC0(%rip), %rsi
	movl	$1, %edx
#APP
# 43 "startup.c" 1
	syscall
# 0 "" 2
#NO_APP
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	orig_puts, .-orig_puts
	.globl	orig_putc
	.type	orig_putc, @function
orig_putc:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	movl	$1, %eax
	movl	$1, %edi
	leaq	-4(%rbp), %rsi
	movl	$1, %edx
#APP
# 52 "startup.c" 1
	syscall
# 0 "" 2
#NO_APP
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	orig_putc, .-orig_putc
	.globl	orig_open
	.type	orig_open, @function
orig_open:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movq	-24(%rbp), %rdi
	movl	-28(%rbp), %esi
	movl	-32(%rbp), %edx
	movl	$2, %eax
#APP
# 62 "startup.c" 1
	syscall
# 0 "" 2
# 63 "startup.c" 1
	movl %eax, %eax;
# 0 "" 2
#NO_APP
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	orig_open, .-orig_open
	.globl	orig_close
	.type	orig_close, @function
orig_close:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %edi
	movl	$3, %eax
#APP
# 74 "startup.c" 1
	syscall
# 0 "" 2
# 75 "startup.c" 1
	movl %eax, %eax;
# 0 "" 2
#NO_APP
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	orig_close, .-orig_close
	.globl	orig_read
	.type	orig_read, @function
orig_read:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movl	-20(%rbp), %edi
	movq	-32(%rbp), %rsi
	movq	-40(%rbp), %rdx
	movl	$0, %eax
#APP
# 88 "startup.c" 1
	syscall
# 0 "" 2
# 89 "startup.c" 1
	movl %eax, %eax;
# 0 "" 2
#NO_APP
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	cltq
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	orig_read, .-orig_read
	.globl	orig_getc
	.type	orig_getc, @function
orig_getc:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	-20(%rbp), %eax
	leaq	-9(%rbp), %rcx
	movl	$1, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	orig_read@PLT
	movzbl	-9(%rbp), %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L15
	call	__stack_chk_fail@PLT
.L15:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	orig_getc, .-orig_getc
	.globl	orig_fstat
	.type	orig_fstat, @function
orig_fstat:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	-20(%rbp), %edi
	movq	-32(%rbp), %rsi
	movl	$5, %eax
#APP
# 107 "startup.c" 1
	syscall
# 0 "" 2
# 108 "startup.c" 1
	movl %eax, %eax;
# 0 "" 2
#NO_APP
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	orig_fstat, .-orig_fstat
	.globl	orig_getline
	.type	orig_getline, @function
orig_getline:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movq	%rcx, -40(%rbp)
	movl	%r8d, -44(%rbp)
	movl	$0, -4(%rbp)
	movb	$0, -5(%rbp)
	jmp	.L19
.L21:
	movl	-44(%rbp), %eax
	movl	%eax, %edi
	call	orig_getc@PLT
	movb	%al, -5(%rbp)
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-5(%rbp), %eax
	movb	%al, (%rdx)
	addl	$1, -4(%rbp)
.L19:
	cmpb	$10, -5(%rbp)
	je	.L20
	movl	-28(%rbp), %eax
	subl	$2, %eax
	cmpl	%eax, -4(%rbp)
	jl	.L21
.L20:
	movq	-40(%rbp), %rax
	movl	(%rax), %edx
	movl	-4(%rbp), %eax
	addl	%eax, %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%eax, -32(%rbp)
	jge	.L22
	movl	$-1, %eax
	jmp	.L23
.L22:
	movl	-4(%rbp), %eax
.L23:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	orig_getline, .-orig_getline
	.section	.rodata
.LC1:
	.string	"Initializing libinterposer..."
.LC2:
	.string	"symbols"
.LC3:
	.string	"Failed opening symbols file."
.LC4:
	.string	"Reading symbol file...\n"
	.align 8
.LC5:
	.string	"Warning: symbol address is NULL!"
.LC6:
	.string	"\nDone."
	.align 8
.LC7:
	.string	"Done resolving original function addresses for trampolines."
	.text
	.globl	init
	.type	init, @function
init:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$704, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC1(%rip), %rdi
	call	orig_puts@PLT
	movl	$0, %edx
	movl	$0, %esi
	leaq	.LC2(%rip), %rdi
	call	orig_open@PLT
	movl	%eax, -692(%rbp)
	cmpl	$-1, -692(%rbp)
	jne	.L25
	leaq	.LC3(%rip), %rdi
	call	orig_puts@PLT
	movl	$1, %edi
	call	exit@PLT
.L25:
	leaq	-672(%rbp), %rdx
	movl	-692(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	orig_fstat@PLT
	movq	-624(%rbp), %rax
	movq	%rax, -680(%rbp)
	movl	$0, -700(%rbp)
	movl	$0, -688(%rbp)
	movl	$1024, -684(%rbp)
	movl	$0, -696(%rbp)
	leaq	.LC4(%rip), %rdi
	call	orig_puts@PLT
	jmp	.L26
.L28:
	movl	-688(%rbp), %eax
	subl	$1, %eax
	cltq
	movb	$0, -528(%rbp,%rax)
	leaq	-528(%rbp), %rax
	movq	%rax, %rdi
	call	orig_puts@PLT
	leaq	-528(%rbp), %rax
	movq	%rax, %rsi
	movq	$-1, %rdi
	call	dlsym@PLT
	movq	%rax, %rcx
	movq	trampoline_original_addr@GOTPCREL(%rip), %rax
	movl	-696(%rbp), %edx
	movslq	%edx, %rdx
	movq	%rcx, (%rax,%rdx,8)
	movq	trampoline_original_addr@GOTPCREL(%rip), %rax
	movl	-696(%rbp), %edx
	movslq	%edx, %rdx
	movq	(%rax,%rdx,8), %rax
	testq	%rax, %rax
	jne	.L27
	leaq	.LC5(%rip), %rdi
	call	orig_puts@PLT
.L27:
	addl	$1, -696(%rbp)
.L26:
	movq	-680(%rbp), %rax
	movl	%eax, %esi
	movl	-692(%rbp), %ecx
	leaq	-700(%rbp), %rdx
	leaq	-528(%rbp), %rax
	movl	%ecx, %r8d
	movq	%rdx, %rcx
	movl	%esi, %edx
	movl	$512, %esi
	movq	%rax, %rdi
	call	orig_getline@PLT
	movl	%eax, -688(%rbp)
	cmpl	$-1, -688(%rbp)
	jne	.L28
	leaq	.LC6(%rip), %rdi
	call	orig_puts@PLT
	leaq	.LC7(%rip), %rdi
	call	orig_puts@PLT
	movl	-692(%rbp), %eax
	movl	%eax, %edi
	call	orig_close@PLT
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L30
	call	__stack_chk_fail@PLT
.L30:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	init, .-init
	.section	.init_array,"aw"
	.align 8
	.quad	init
