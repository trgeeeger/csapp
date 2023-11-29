#include<stdio.h>
#include<string.h>
#include "headers/cpu.h"
#include "headers/memory.h"
#include "headers/common.h"

#define MAX_NUM_INSTRUCTION_CYCLE 100

static void TestAddFunctionCallAndComputation();
static void TestSumFunctionCallAndComputation();


void TestParsingOperand();
void TestParsingInstruction();
// symbols from isa and sram
void print_register(core_t *cr);
void print_stack(core_t *cr);

char a[][20] = {
    "0",
    "-0",
    "0x0",
    "1234",
    "0x1234",
    "0xabcd",
    "-0xabcd",
    "-1234",
    "2147483647",
    "-2147483648",
    "0x8000000000000000",
    "0xffffffffffffffff"
};

int main()
{
    //TestAddFunctionCallAndComputation();
    //TestParsingInstruction();
    //TestAddFunctionCallAndComputation();
    TestSumFunctionCallAndComputation();
    return 0;
}

static void TestAddFunctionCallAndComputation()
{
    ACTIVE_CORE = 0x0;

    core_t *ac = (core_t *)&cores[ACTIVE_CORE];

    // init state
    ac->reg.rax = 0xabcd;
    ac->reg.rbx = 0x8000670;
    ac->reg.rcx = 0x8000670;
    ac->reg.rdx = 0x12340000;
    ac->reg.rsi = 0x7ffffffee208;
    ac->reg.rdi = 0x1;
    ac->reg.rbp = 0x7ffffffee110;
    ac->reg.rsp = 0x7ffffffee0f0;

    ac->flags.__flags_value = 0;

    write64bits_dram(va2pa(0x7ffffffee110, ac), 0x0000000000000000, ac);    // rbp
    write64bits_dram(va2pa(0x7ffffffee108, ac), 0x0000000000000000, ac);
    write64bits_dram(va2pa(0x7ffffffee100, ac), 0x0000000012340000, ac);
    write64bits_dram(va2pa(0x7ffffffee0f8, ac), 0x000000000000abcd, ac);
    write64bits_dram(va2pa(0x7ffffffee0f0, ac), 0x0000000000000000, ac);    // rsp

    // 2 before call
    // 3 after call before push
    // 5 after rbp
    // 13 before pop
    // 14 after pop before ret
    // 15 after ret
    /*
    char assembly[19][MAX_INSTRUCTION_CHAR] = {
        "push   %rbp",              // 0
        "mov    %rsp,%rbp",         // 1
        "sub    $0x10,%rsp",        // 2
        "mov    %rdi,-0x8(%rbp)",   // 3
        "cmpq   $0x0,-0x8(%rbp)",   // 4
        "jne    4004f0",            // 5
        "mov    $0x0,%eax",         // 6
        "jmp    40050a",            // 7
        "mov    -0x8(%rbp),%rax",   // 8
        "sub    $0x1,%rax",         // 9
        "mov    %rax,%rdi",         // 10
        "callq  4004d6",            // 11
        "mov    -0x8(%rbp),%rax",   // 12
        "add    %rdx,%rax",         // 13
        "leaveq",                   // 14
        "retq",                     //15
        "mov    $0x3,%edi",         //16
        "callq  4004d6",            //17
        "mov    %rax,-0x8(%rbp)"    //18
    };
    */
    char assembly[15][MAX_INSTRUCTION_CHAR] = {
    "push   %rbp",              // 0
    "mov    %rsp,%rbp",         // 1
    "mov    %rdi,-0x18(%rbp)",  // 2
    "mov    %rsi,-0x20(%rbp)",  // 3
    "mov    -0x18(%rbp),%rdx",  // 4
    "mov    -0x20(%rbp),%rax",  // 5
    "add    %rdx,%rax",         // 6
    "mov    %rax,-0x8(%rbp)",   // 7
    "mov    -0x8(%rbp),%rax",   // 8
    "pop    %rbp",              // 9
    "retq",                     // 10
    "mov    %rdx,%rsi",         // 11
    "mov    %rax,%rdi",         // 12
    "callq  0",                 // 13
    "mov    %rax,-0x8(%rbp)",   // 14
    };
    ac->rip = (uint64_t)&assembly[11];
    //printf(" addr ==> %p\n", &assembly[0]);
    sprintf(assembly[13], "callq  $%p", &assembly[0]);

    printf("begin\n");
    int time = 0;
    while (time < 15)
    {
        instruction_cycle(ac);
        print_register(ac);
        print_stack(ac);
        time ++;
    } 

    // gdb state ret from func
    int match = 1;
    match = match && ac->reg.rax == 0x1234abcd;
    match = match && ac->reg.rbx == 0x8000670;
    match = match && ac->reg.rcx == 0x8000670;
    match = match && ac->reg.rdx == 0xabcd;
    match = match && ac->reg.rsi == 0x12340000;
    match = match && ac->reg.rdi == 0xabcd;
    match = match && ac->reg.rbp == 0x7ffffffee110;
    match = match && ac->reg.rsp == 0x7ffffffee0f0;

    if (match)
    {
        printf("register match\n");
    }
    else
    {
        printf("register mismatch\n");
    }

    match = match && (read64bits_dram(va2pa(0x7ffffffee110, ac), ac) == 0x0000000000000000); // rbp
    match = match && (read64bits_dram(va2pa(0x7ffffffee108, ac), ac) == 0x000000001234abcd);
    match = match && (read64bits_dram(va2pa(0x7ffffffee100, ac), ac) == 0x0000000012340000);
    match = match && (read64bits_dram(va2pa(0x7ffffffee0f8, ac), ac) == 0x000000000000abcd);
    match = match && (read64bits_dram(va2pa(0x7ffffffee0f0, ac), ac) == 0x0000000000000000); // rsp

    if (match)
    {
        printf("memory match\n");
    }
    else
    {
        printf("memory mismatch\n");
    }
}
static void TestSumFunctionCallAndComputation()
{
    ACTIVE_CORE = 0x0;

    core_t *ac = (core_t *)&cores[ACTIVE_CORE];

    // init state
    ac->reg.rax = 0x800630;
    ac->reg.rbx = 0x0;
    ac->reg.rcx = 0x8000650;
    ac->reg.rdx = 0x7ffffffee328;
    ac->reg.rsi = 0x7ffffffee318;
    ac->reg.rdi = 0x1;
    ac->reg.rbp = 0x7ffffffee230;
    ac->reg.rsp = 0x7ffffffee220;

    ac->flags.__flags_value = 0;

    write64bits_dram(va2pa(0x7ffffffee230, ac), 0x0000000000000650, ac);    // rbp
    write64bits_dram(va2pa(0x7ffffffee228, ac), 0x0000000000000000, ac);
    write64bits_dram(va2pa(0x7ffffffee220, ac), 0x00007ffffffee310, ac);

    // 2 before call
    // 3 after call before push
    // 5 after rbp
    // 13 before pop
    // 14 after pop before ret
    // 15 after ret
    
    char assembly[19][MAX_INSTRUCTION_CHAR] = {
        "push   %rbp",              // 0
        "mov    %rsp,%rbp",         // 1
        "sub    $0x10,%rsp",        // 2
        "mov    %rdi,-0x8(%rbp)",   // 3
        "cmpq   $0x0,-0x8(%rbp)",   // 4
        "jne    0x400200",          // 5  0x400200
        "mov    $0x0,%eax",         // 6  
        "jmp    0x400380",          // 7   0x400380 14
        "mov    -0x8(%rbp),%rax",   // 8
        "sub    $0x1,%rax",         // 9
        "mov    %rax,%rdi",         // 10
        "callq  0x400000",          // 11  0x00400000
        "mov    -0x8(%rbp),%rax",   // 12
        "add    %rdx,%rax",         // 13
        "leaveq",                   // 14
        "retq",                     // 15
        "mov    $0x3,%edi",         // 16
        "callq  0x00400000",        // 17
        "mov    %rax,-0x8(%rbp)"    // 18
    };
    //printf("addr ==> %p\n", &assembly[0]);
    for (int i = 0; i < 19; ++i)
    {
        writeinst_dram(va2pa(i * 0x40 + 0x00400000, ac), assembly[i], ac);
    }

    ac->rip = MAX_INSTRUCTION_CHAR * sizeof(char) * 16 + 0x00400000;
    printf("rip ==> %lx\n", ac->rip);
    //printf(" addr ==> %p\n", &assembly[0]);
    //uint64_t begin = MAX_INSTRUCTION_CHAR * sizeof(char) * 0 + 0x00400000;
    //sprintf(assembly[13], "callq  $%lx", begin);

    printf("begin\n");
    int time = 0;
    while (ac->rip <= 18 * 0x40 + 0x00400000 && 
            time < MAX_NUM_INSTRUCTION_CYCLE)
    {
        instruction_cycle(ac);
        print_register(ac);
        print_stack(ac);
        time ++;
    } 

    // gdb state ret from func
    int match = 1;
    match = match && ac->reg.rax == 0x800630;
    match = match && ac->reg.rbx == 0x0;
    match = match && ac->reg.rcx == 0x8000650;
    match = match && ac->reg.rdx == 0x7ffffffee328;
    match = match && ac->reg.rsi == 0x7ffffffee318;
    match = match && ac->reg.rdi == 0x1;
    match = match && ac->reg.rbp == 0x7ffffffee230;
    match = match && ac->reg.rsp == 0x7ffffffee220;

    if (match)
    {
        printf("register match\n");
    }
    else
    {
        printf("register mismatch\n");
    }

    match = match && (read64bits_dram(va2pa(0x7ffffffee230, ac), ac) == 0x0000000000000650); // rbp
    match = match && (read64bits_dram(va2pa(0x7ffffffee228, ac), ac) == 0x0000000000000000);
    match = match && (read64bits_dram(va2pa(0x7ffffffee220, ac), ac) == 0x00007ffffffee310);
    if (match)
    {
        printf("memory match\n");
    }
    else
    {
        printf("memory mismatch\n");
    }
}