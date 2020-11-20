#ifndef LED_MMAP
#define LED_MMAP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define FPGA_BASE_ADDRESS 0x08000000//fpga ����� address��, LED�� �����ּҸ� �����ϱ� ���� base address
#define LED_ADDR 0x16


void led_mmap(unsigned char);

#endif