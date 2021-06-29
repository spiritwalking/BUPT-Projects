#include "definition.h"

extern int pc[2], fr[2];
extern int gr[2][9];
extern int dataSegment[DATAMAX];
extern int codeSegment[CODEMAX];
extern HANDLE hMutex1;

void stop(Instruction cache)
{
    pc[cache.id - 1] += 4;
}

void mov(Instruction cache)
{
    int offset = 0, i = cache.id - 1;
    if (cache.rt == 0)
    {
        gr[i][cache.rs] = cache.im;
    }
    else if (cache.rt >= 5)
    {
        offset = (gr[i][cache.rt] - 16384) / 2;
        gr[i][cache.rs] = dataSegment[offset];
    }
    else
    {
        offset = (gr[i][cache.rs] - 16384) / 2;
        dataSegment[offset] = gr[i][cache.rt];
    }
    pc[i] += 4;
}

void cmp(Instruction cache)
{
    int i = cache.id - 1;
    int offset = (gr[i][cache.rt] - 16384) / 2; //计算地址偏移量
    if (cache.rt == 0)
    {
        if (gr[i][cache.rs] > cache.im)
            fr[i] = 1;
        else if (gr[i][cache.rs] == cache.im)
            fr[i] = 0;
        else
            fr[i] = -1;
    }
    else
    {
        if (gr[i][cache.rs] > dataSegment[offset])
            fr[i] = 1;
        else if (gr[i][cache.rs] == dataSegment[offset])
            fr[i] = 0;
        else
            fr[i] = -1;
    }
    pc[i] += 4;
}

void jmp(Instruction cache)
{
    int i = cache.id - 1;
    if (cache.rt == 0)
        pc[i] += cache.im;
    else if (cache.rt == 1 && fr[i] == 0)
        pc[i] += cache.im;
    else if (cache.rt == 2 && fr[i] == 1)
        pc[i] += cache.im;
    else if (cache.rt == 3 && fr[i] == -1)
        pc[i] += cache.im;
    else
        pc[i] += 4;
}

//算数运算指令
void arithmetic_operation(Instruction cache)
{
    int i = cache.id - 1;
    int offset = (gr[i][cache.rt] - 16384) / 2;
    if (cache.op == 2)
    {
        if (cache.rt == 0)
            gr[i][cache.rs] += cache.im;
        else
            gr[i][cache.rs] += dataSegment[offset];
    }
    else if (cache.op == 3)
    {
        if (cache.rt == 0)
            gr[i][cache.rs] -= cache.im;
        else
            gr[i][cache.rs] -= dataSegment[offset];
    }
    else if (cache.op == 4)
    {
        if (cache.rt == 0)
            gr[i][cache.rs] *= cache.im;
        else
            gr[i][cache.rs] *= dataSegment[offset];
    }
    else
    {
        if (cache.rt == 0)
            gr[i][cache.rs] /= cache.im;
        else
            gr[i][cache.rs] /= dataSegment[offset];
    }
    pc[i] += 4;
}

//逻辑运算指令
void logic_operation(Instruction cache)
{
    int i = cache.id - 1;
    int offset = (gr[i][cache.rt] - 16384) / 2;
    if (cache.op == 6)
    {
        if (cache.rt == 0)
            gr[i][cache.rs] = gr[i][cache.rs] && cache.im;
        else
            gr[i][cache.rs] = gr[i][cache.rs] && dataSegment[offset];
    }
    else if (cache.op == 7)
    {
        if (cache.rt == 0)
            gr[i][cache.rs] = gr[i][cache.rs] || cache.im;
        else
            gr[i][cache.rs] = gr[i][cache.rs] || dataSegment[offset];
    }
    else
    {
        if (cache.rt == 0)
            gr[i][cache.rs] = !gr[i][cache.rs];
        else
            dataSegment[offset] = !dataSegment[offset];
    }
    pc[i] += 4;
}

//标准输入输出指令
void standard_io(Instruction cache)
{
    int i = cache.id - 1;
    if (cache.op == 11)
    {
        printf("in:\n");
        scanf("%d", &gr[i][cache.rs]);
    }
    else
        printf("id = %d    out: %d\n", cache.id, gr[i][cache.rs]);

    pc[i] += 4;
}

void para_exe(Instruction cache)
{
    if (cache.op == 13)
        WaitForSingleObject(hMutex1, INFINITE); //锁住内存(等待有信号状态)
    else if (cache.op == 14)
        ReleaseMutex(hMutex1); //释放内存(释放互斥对象访问权)
    else
        Sleep(cache.im); //休眠立即数

    pc[cache.id - 1] += 4;
}
