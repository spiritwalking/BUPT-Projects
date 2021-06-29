#include "definition.h"

extern int pc, ir, fr;
extern int gr[9];
extern Instruction cache;
extern int dataSegment[1024];
extern int codeSegment[512];

void stop()
{
    pc += 4;
}

void mov()
{
    int offset = 0;
    if (cache.rt == 0)
    {
        gr[cache.rs] = cache.im;
    }
    else if (cache.rt >= 5)
    {
        offset = (gr[cache.rt] - 16384) / 2;
        gr[cache.rs] = dataSegment[offset];
    }
    else
    {
        offset = (gr[cache.rs] - 16384) / 2;
        dataSegment[offset] = gr[cache.rt];
    }
    pc += 4;
}

void cmp()
{
    int offset = (gr[cache.rt] - 16384) / 2; //计算地址偏移量
    if (cache.rt == 0)
    {
        if (gr[cache.rs] > cache.im)
            fr = 1;
        else if (gr[cache.rs] == cache.im)
            fr = 0;
        else
            fr = -1;
    }
    else
    {
        if (gr[cache.rs] > dataSegment[offset])
            fr = 1;
        else if (gr[cache.rs] == dataSegment[offset])
            fr = 0;
        else
            fr = -1;
    }
    pc += 4;
}

void jmp()
{
    if (cache.rt == 0)
        pc += cache.im;
    else if (cache.rt == 1 && fr == 0)
        pc += cache.im;
    else if (cache.rt == 2 && fr == 1)
        pc += cache.im;
    else if (cache.rt == 3 && fr == -1)
        pc += cache.im;
    else
        pc += 4;
}

//算数运算指令
void arithmetic_operation()
{
    int offset = (gr[cache.rt] - 16384) / 2;
    if (cache.op == 2)
    {
        if (cache.rt == 0)
            gr[cache.rs] += cache.im;
        else
            gr[cache.rs] += dataSegment[offset];
    }
    else if (cache.op == 3)
    {
        if (cache.rt == 0)
            gr[cache.rs] -= cache.im;
        else
            gr[cache.rs] -= dataSegment[offset];
    }
    else if (cache.op == 4)
    {
        if (cache.rt == 0)
            gr[cache.rs] *= cache.im;
        else
            gr[cache.rs] *= dataSegment[offset];
    }
    else
    {
        if (cache.rt == 0)
            gr[cache.rs] /= cache.im;
        else
            gr[cache.rs] /= dataSegment[offset];
    }
    pc += 4;
}

//逻辑运算指令
void logic_operation()
{
    int offset = (gr[cache.rt] - 16384) / 2;
    if (cache.op == 6)
    {
        if (cache.rt == 0)
            gr[cache.rs] = gr[cache.rs] && cache.im;
        else
            gr[cache.rs] = gr[cache.rs] && dataSegment[offset];
    }
    else if (cache.op == 7)
    {
        if (cache.rt == 0)
            gr[cache.rs] = gr[cache.rs] || cache.im;
        else
            gr[cache.rs] = gr[cache.rs] || dataSegment[offset];
    }
    else
    {
        if (cache.rt == 0)
            gr[cache.rs] = !gr[cache.rs];
        else
            dataSegment[offset] = !dataSegment[offset];
    }
    pc += 4;
}

//标准输入输出指令
void standard_io()
{
    if (cache.op == 11)
    {
        printf("in:\n");
        scanf("%d", &gr[cache.rs]);
    }
    else
    {
        printf("out: %d\n", gr[cache.rs]);
    }
    pc += 4;
}