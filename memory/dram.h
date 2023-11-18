#ifndef _DRAM_H_
#define _DRAM_H_

#include <stdint.h>

#define MM_LEN 1000

uint8_t mm[MM_LEN];  //物理内存  physical memory

uint64_t read64bits_dram(uint64_t paddr);
void write64bits_dram(uint64_t paddr, uint64_t data);


void print_register();
void print_stack();

#endif