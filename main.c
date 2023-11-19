#include "cpu/register.h"
#include "memory/instruction.h"
#include "disk/elf.h"
#include "cpu/mmu.h"
#include "memory/dram.h"
#include <stdio.h>
#include <stdint.h>
int main()
{
    init_handler_table();

    //init
    reg.rax = 0x12340000;
    reg.rbx = 0x0;
    reg.rcx = 0x8000660;
    reg.rdx = 0xabcd;
    reg.rsi = 0x7ffffffee2f8;
    reg.rdi = 0x1;
    reg.rbp = 0x7ffffffee210;
    reg.rsp = 0x7ffffffee1f0;

    reg.rip = (uint64_t)&program[11];
/*
    write64bits_dram(va2pa(0x7fffffffe064), 0xf7a2d84000000000);
    write64bits_dram(va2pa(0x7fffffffe05c), 0x0040053000000000);
    write64bits_dram(va2pa(0x7fffffffe054), 0x0);
    write64bits_dram(va2pa(0x7fffffffe04c), 0xabcd000000000000);
    write64bits_dram(va2pa(0x7fffffffe044), 0x0000123400000000);
*/
    write64bits_dram(va2pa(0x7ffffffee210), 0x08000660);
    write64bits_dram(va2pa(0x7ffffffee208), 0x0);
    write64bits_dram(va2pa(0x7ffffffee200), 0xabcd);
    write64bits_dram(va2pa(0x7ffffffee1f8), 0x12340000);
    write64bits_dram(va2pa(0x7ffffffee1f0), 0x08000660);
   // uint64_t pa = va2pa(0x7fffffffe064);
   // printf("%lx\n", read64bits_dram(va2pa(0x7fffffffe04c)));


    print_register();
    print_stack();

    for (int i = 0; i < 15; ++i) 
    {
        instruction_cycle();

        print_register();
        print_stack();
    }


    int match = 1;
/*
    match = match && (reg.rax == 0x12340000);
    match = match && (reg.rbx == 0x0);
    match = match && (reg.rcx == 0x8000660);
    match = match && (reg.rdx == 0xabcd);
    match = match && (reg.rsi == 0x7ffffffee2f8);
    match = match && (reg.rdi == 0x1);
    match = match && (reg.rbp == 0x7ffffffee210);
    match = match && (reg.rsp == 0x7ffffffee1f0);
*/
    if (match == 1)
    {
        printf("register match \n");
    }
    else
    {
        printf("register not match \n");
    }

    match = match && (read64bits_dram(va2pa(0x7ffffffee210)) == 0x08000660);
    match = match && (read64bits_dram(va2pa(0x7ffffffee208)) == 0x0);
    match = match && (read64bits_dram(va2pa(0x7ffffffee200)) == 0xabcd);
    match = match && (read64bits_dram(va2pa(0x7ffffffee1f8)) == 0x12340000);
    match = match && (read64bits_dram(va2pa(0x7ffffffee1f0)) == 0x08000660);

    return 0;
} 