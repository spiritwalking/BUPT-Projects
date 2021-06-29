#ifndef _DEFINITION_H
#define _DEFINITION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Instrution
{
    int op;                   //操作码
    int rs;                   //源操作数寄存器1
    int rt;                   //源操作数寄存器2
    int im;                   //立即数
    int address;              //指令地址
    int dec_instruction_high; //十进制高16位指令
    char bin_instruction[34]; //二进制指令
    struct Instrution *next;
} * LinkInstruction, Instruction;

LinkInstruction initInstruction();
void loadCode(LinkInstruction code, FILE *fp);
void runCode(LinkInstruction code);
int B2D_trasition(char *s, int begin, int end, int mode);
void printStatus();

void stop();
void mov();
void cmp();
void jmp();
void arithmetic_operation();
void logic_operation();
void standard_io();

#endif