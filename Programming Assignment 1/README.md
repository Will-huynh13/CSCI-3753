# Programmming Assignment 1

### Name: WIll Huynh : wihu4176@coloroado.edu

This assignment will go over how to compile and install modern Linux kernel, as well as custom system calls.

### The C code included in this file will test both Helloworld and cs3753 system calls 

```
syscallTest.c
```
How to compile: gcc syscallTest.c -o test

### Kernel Files that were changed

kernel files are in the appropriate paths listed below: 
```
arch/x86/kernel/simple_add.c
arch/x86/kernel/Makefile
arch/x86/syscalls/syscall_64.tbl
includes/linux/syscalls.h

```

### System Log file
```
dmesg
```
(this file can be obtained by using `dmesg >> log.txt`)
