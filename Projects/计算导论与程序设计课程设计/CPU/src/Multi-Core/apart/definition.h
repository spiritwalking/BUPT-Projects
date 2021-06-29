#ifndef _DEFINITION_H
#define _DEFINITION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <windows.h>
#define DATAMAX 1024
#define CODEMAX 512
#define TICKETS 100
#define CODEBEGIN_1 0   //代码表1内存起始位置
#define CODEBEGIN_2 256 //代码表2内存起始位置

typedef struct Instrution
{
    int op;                   //操作码
    int rs;                   //源操作数寄存器1
    int rt;                   //源操作数寄存器2
    int im;                   //立即数
    int address;              //指令地址
    int dec_instruction_high; //十进制高16位指令
    char bin_instruction[34]; //二进制指令
    int id;                   //线程id
    struct Instrution *next;
} * LinkInstruction, Instruction;

LinkInstruction initInstruction();
void loadCode(LinkInstruction code, FILE *fp, int begin);
unsigned __stdcall runCode(void *ptr);
int B2D_trasition(char *s, int begin, int end, int mode);
void printStatus(Instruction cache);
void printMemory();

void stop(Instruction cache);
void mov(Instruction cache);
void cmp(Instruction cache);
void jmp(Instruction cache);
void arithmetic_operation(Instruction cache);
void logic_operation(Instruction cache);
void standard_io(Instruction cache);
void para_exe(Instruction cache);

#endif