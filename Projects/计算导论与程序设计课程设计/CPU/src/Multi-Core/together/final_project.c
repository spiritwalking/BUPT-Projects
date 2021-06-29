/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: Simulate Von Neumann multi-core CPU
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

int pc[2] = {0}, ir[2] = {0}, fr[2] = {0}; //系统寄存器
int gr[2][9] = {0};                        //通用寄存器
int dataSegment[DATAMAX] = {0};            //数据段(每个数代表2Byte)
int codeSegment[CODEMAX] = {0};            //代码段(每个数代表4Byte)
HANDLE hMutex1, hMutex2;                   //互斥对象句柄

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

int main(void)
{
    LinkInstruction code1 = initInstruction();
    LinkInstruction code2 = initInstruction();
    code1->id = 1;
    code2->id = 2;
    unsigned ThreadID1 = 1, ThreadID2 = 2;
    pc[0] = CODEBEGIN_1;
    pc[1] = CODEBEGIN_2; //初始化

    HANDLE thread1, thread2;
    FILE *codeFile1 = fopen("dict1.dic", "r");
    FILE *codeFile2 = fopen("dict2.dic", "r");

    loadCode(code1, codeFile1, 0);
    loadCode(code2, codeFile2, 256); //载入两个代码表
    dataSegment[0] = TICKETS;        //将100存在内存地址16384处
    thread1 = (HANDLE)_beginthreadex(NULL, 0, runCode, (void *)code1, 0, &ThreadID1);
    thread2 = (HANDLE)_beginthreadex(NULL, 0, runCode, (void *)code2, 0, &ThreadID2); //创建两个进程

    fclose(codeFile1);
    fclose(codeFile2);
    WaitForSingleObject(thread1, INFINITE);
    CloseHandle(thread1);
    WaitForSingleObject(thread2, INFINITE);
    CloseHandle(thread2);
    printMemory(); //输出内存状态
    return 0;
}

//初始化节点
LinkInstruction initInstruction()
{
    LinkInstruction ptr = (LinkInstruction)malloc(sizeof(Instruction));
    memset(ptr, 0, sizeof(Instruction));
    return ptr;
}

//载入代码
void loadCode(LinkInstruction code, FILE *fp, int begin)
{
    LinkInstruction node, link = code;
    char tmp[34] = {0}, c = 0;
    int i = begin / 4;
    int id;
    if (code->id == 1)
        id = 1;
    else
        id = 2;

    fgets(tmp, 34, fp);
    while (!feof(fp))
    {
        node = initInstruction();
        tmp[32] = 0;

        //载入具体指令
        node->op = B2D_trasition(tmp, 0, 7, 0);
        node->rs = B2D_trasition(tmp, 8, 11, 0);
        node->rt = B2D_trasition(tmp, 12, 15, 0);
        node->im = B2D_trasition(tmp, 16, 31, 1);
        node->address = 4 * i;
        node->dec_instruction_high = B2D_trasition(tmp, 0, 15, 0);
        node->id = id;
        strcpy(node->bin_instruction, tmp);

        codeSegment[i++] = B2D_trasition(tmp, 0, 31, 0);
        link->next = node;
        link = link->next;
        fgets(tmp, 34, fp);
    }
}

//执行指令
unsigned __stdcall runCode(void *ptr)
{
    hMutex1 = CreateMutex(NULL, FALSE, "memory");
    hMutex2 = CreateMutex(NULL, FALSE, "output");
    LinkInstruction code = (LinkInstruction)ptr;
    LinkInstruction cur = code->next;
    Instruction cache;
    int i;

    while (cur)
    {
        cache = *cur;                                  //载入高速缓存器cache
        ir[cache.id - 1] = cache.dec_instruction_high; //载入指令寄存器ir
        switch (cache.op)
        {
        case 0:
            stop(cache);
            break;
        case 1:
            mov(cache);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            arithmetic_operation(cache);
            break;
        case 6:
        case 7:
        case 8:
            logic_operation(cache);
            break;
        case 9:
            cmp(cache);
            break;
        case 10:
            jmp(cache);
            break;
        case 11:
        case 12:
            standard_io(cache);
            break;
        case 13:
        case 14:
        case 15:
            para_exe(cache);
            break;

        default:
            break;
        }
        WaitForSingleObject(hMutex2, INFINITE); //避免多线程输出不完整
        if (cache.op != 12)
        {
            printf("\n");
            printStatus(cache);
        }
        ReleaseMutex(hMutex2);

        if (!cur->next) //指令执行顺序控制
            break;
        if (cur->next->address == pc[cache.id - 1])
        {
            cur = cur->next;
        }
        else
        {
            cur = code->next;
            while (cur->address != pc[cache.id - 1])
                cur = cur->next;
        }
    }
    _endthreadex(0);
}

int B2D_trasition(char *s, int begin, int end, int mode)
{
    int dec = 0, weight = 1, i;
    //计算原码
    if (mode == 0)
    {
        for (i = end; i >= begin; i--)
        {
            dec += (s[i] - '0') * weight;
            weight *= 2;
        }
    }
    //计算补码
    else
    {
        if (s[begin] == '0')
        {
            for (i = end; i >= begin; i--)
            {
                dec += (s[i] - '0') * weight;
                weight *= 2;
            }
        }
        else
        {
            for (i = end; i >= begin; i--)
            {
                dec += (s[i] - '0') * weight;
                weight *= 2;
            }
            dec = -((int)pow(2, end - begin + 1) - dec);
        }
    }
    return dec;
}

//输出寄存器状态
void printStatus(Instruction cache)
{
    int i = cache.id - 1;
    printf("id = %d\n", cache.id);
    printf("ip = %d\n", pc[i]);
    printf("flag = %d\n", fr[i]);
    printf("ir = %d\n", ir[i]);
    printf("ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n", gr[i][1], gr[i][2], gr[i][3], gr[i][4]);
    printf("ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n", gr[i][5], gr[i][6], gr[i][7], gr[i][8]);
}

//输出内存状态
void printMemory()
{
    int i;
    printf("\n");
    printf("codeSegment :\n");
    for (i = 0; i < 128; i++)
    {
        printf("%d", codeSegment[i]);
        if ((i + 1) % 8 == 0)
            printf("\n");
        else
            printf(" ");
    }
    printf("\n");
    printf("dataSegment :\n");
    for (i = 0; i < 256; i++)
    {
        printf("%d", dataSegment[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
        else
            printf(" ");
    }
}

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
