#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <errno.h>
#include <link.h>

#define BUFFER_SIZE 512

unsigned long* trampoline_original_addr[1024];		// the original address

/*
 * The implementation of libc functions using only syscalls.
 * This is necessary since the libc wrappers might use functions 
 * where the original function addresses might not yet be resolved.
 * NOTE: These are unsafe yet and don't use error checking!
 * The definition here might be not consistent with the manpages,
 * the reason is that e.g. malloc() is not accessible here.
 */

int orig_puts(const char* s){
	int len = 0;
	char c = s[0];
	while (c != '\0'){
		c = s[len];
		len++;
	}
	register int    syscall_no  asm("rax") = 1;	// sys_write
	register int    arg1        asm("rdi") = 1;
	register char*  arg2        asm("rsi") = s;
	register int    arg3        asm("rdx") = len;
	asm("syscall");
	register int    nl_syscall_no  asm("rax") = 1;
	register int    nl_arg1        asm("rdi") = 1;
	register char*  nl_arg2        asm("rsi") = "\n";
	register int    nl_arg3        asm("rdx") = 1;
	asm("syscall");
	return 0;
}

int orig_putc(const char c){
	register int    nl_syscall_no  asm("rax") = 1;
	register int    nl_arg1        asm("rdi") = 1;
	register char*  nl_arg2        asm("rsi") = &c;
	register int    nl_arg3        asm("rdx") = 1;
	asm("syscall");
	return 0;
}

int orig_open(const char* pathname, int flags, mode_t mode){
	int fd;
	register char*  	arg1       	asm("rdi") = pathname;
	register int  		arg2        asm("rsi") = flags;
	register mode_t    	arg3        asm("rdx") = mode;
	register int    	syscall_no  asm("rax") = 2;	// sys_open <- move sycall_no here, because GCC would override RAX
	asm("syscall");
	asm ("movl %%eax, %0;" 	// $11 is the value rax to assign to %0 (related to fd)
    :"=r"(fd) 				// %0 is related fd
    :
    :);
	return fd;
}

int orig_close(unsigned int fd){
	int error_val;
	register unsigned int   arg1        asm("rdi") = fd;
	register int    		syscall_no  asm("rax") = 3; // sys_close <- move sycall_no here, because GCC would override RAX
	asm("syscall");
	asm ("movl %%eax, %0;"
    :"=r"(error_val)
    :
    :);
    return (int)error_val;
}

ssize_t orig_read(unsigned int fd, void *buf, size_t count){
	int read_bytes;
	register unsigned int   arg1        asm("rdi") = fd;
	register char*  		arg2        asm("rsi") = buf;
	register size_t    		arg3        asm("rdx") = count;
	register int    		syscall_no  asm("rax") = 0;	// sys_read <- move sycall_no here, because GCC would override RAX
	asm("syscall");
	asm ("movl %%eax, %0;"
    :"=r"(read_bytes)
    :
    :);
    return (int)read_bytes;
}

char orig_getc(int fd){
	char tmp[1];
	orig_read(fd, &tmp, 1);
	return tmp[0];
}

int orig_fstat(unsigned int fd, struct stat *buf){
	int error_val;
	register unsigned int   arg1        asm("rdi") = fd;
	register struct stat*	arg2        asm("rsi") = buf;
	register int    		syscall_no  asm("rax") = 5;	// sys_fstat <- move sycall_no here, because GCC would override RAX
	asm("syscall");
	asm ("movl %%eax, %0;"
    :"=r"(error_val)
    :
    :);
    return (int)error_val;
}

int orig_getline(char* buf, int buf_size, int file_size, int *pos, unsigned int fd){
	int i = 0;
	char c = '\0';
	//orig_puts("getline: ");
	while (c != '\n' && i < buf_size - 2){
		c = orig_getc(fd);
		//orig_putc(c);
		buf[i] = c;
		i++;
	}
	*pos = *pos + i;
	buf[i] = '\0';

	if (*pos >= file_size + 1){
		return -1;
	}
	//orig_putc("\n");
	return i;
}

/*
 *	The initialization function that should run before the main program
 */
__attribute__((constructor)) void init(void) {
	orig_puts("Initializing libinterposer...");

	unsigned int fd = orig_open("symbols", O_RDONLY, 0);
	if (fd == -1){
		orig_puts("Failed opening symbols file.");
		exit(EXIT_FAILURE);
		return;
	}
	struct stat st;
	orig_fstat(fd, &st);
	off_t sz = st.st_size;

	int pos = 0;
	int read_bytes = 0;
	char symbol[BUFFER_SIZE];

	int symbol_count = 1024;


	// resolve the original addresses of the trampoline functions
	int i = 0;
	orig_puts("Reading symbol file...\n");
	while((read_bytes = orig_getline(symbol, BUFFER_SIZE, sz, &pos, fd)) != -1){
		symbol[read_bytes - 1] = '\0';
		orig_puts(symbol);
		trampoline_original_addr[i] = (unsigned long)dlsym(RTLD_NEXT, symbol);
		if (trampoline_original_addr[i] == 0){
			orig_puts("Warning: symbol address is NULL!");
		}
		i++;
	}

	orig_puts("\nDone.");
	orig_puts("Done resolving original function addresses for trampolines.");

	orig_close(fd);

	return;
}

