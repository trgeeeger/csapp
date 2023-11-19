#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_
#include <stdio.h>
#include <stdint.h>

#define NUM_INSTRTYPE 30

typedef enum OP   
{
    mov_reg_reg,   //0 
    add_reg_reg,   //1
    push_reg,      //2
    mov_reg_mem,   //3
    mov_mem_reg,   //4
    pop_reg,       //5
    ret,           //6 
    call           //7

}op_t;

typedef enum OD_TYPE  //各种指令类型
{
    EMPTY,
    IMM, REG,
    MM_IMM, MM_REG, MM_IMM_REG, MM_REG1_REG2,
    MM_IMM_REG1_REG2, MM_REG2_S, MM_IMM_REG2_S, 
    MM_REG1_REG2_S, MM_IMM_REG1_REG2_S
}od_type_t;

typedef struct OD  //指令存在的变量
{
    od_type_t type;

    int64_t imm; //寄存器
    int64_t scal;
    uint64_t *reg1;
    uint64_t *reg2;

}od_t;

typedef struct INSTRUCT_STRUCT
{
     op_t op; //mov, push
     od_t src;
     od_t dst;
     char code[100];
}inst_t;


typedef void (*handler_t)(uint64_t, uint64_t);

handler_t handler_table[NUM_INSTRTYPE];

void instruction_cycle();
void init_handler_table();


void add_reg_reg_handler(uint64_t src, uint64_t dst);
void call_handler(uint64_t src, uint64_t dst);
void mov_reg_reg_handler(uint64_t src, uint64_t dst);
void push_reg_handler(uint64_t src, uint64_t dst);
void mov_reg_mem_handler(uint64_t src, uint64_t dst);
void mov_mem_reg_handler(uint64_t src, uint64_t dst);
void ret_handler(uint64_t src, uint64_t dst);
void pop_reg_handler(uint64_t src, uint64_t dst);

#endif