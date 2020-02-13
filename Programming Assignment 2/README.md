# Programmming Assignment 2

### Name: WIll Huynh<br/>
### Email: wihu4176@coloroado.edu

This assignment will go over Linux Kernel Modules (LKM) and Linux character device drivers.

### The C code included in this file will test both HelloModule and PA2_char_driver

```
CharDriver.c
```
How to compile: gcc CharDriver.c -o test

### LKM installation instructions:

Makefile for PA2_char_device.c:
```
obj-m:=pa2_char_driver.o

```
Compiling the module (From a desktop folder):
```
sudo make -C /lib/modules/4.15.0-33-generic//build M=/home/user/Desktop/PA2/module/ modules

```
Module installation/removal: 
```
sudo insmod pa2_char_driver.ko (installs module)
**use lsmod to list LKMs**
sudo rmmod pa2_char_driver (removes module)
```
Creating character driver (247 unique major number)
```
sudo mknod –m 777 /dev/pa2_character_device c 247 0

```

### System Log file
```
dmesg or tail /var/log/syslog
```
(dmesg can be obtained by using `dmesg >> log.txt`)<br/>
(tail file can be obtained by using `tail /var/log/syslog >> log.txt`)
