/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: Simulate Von Neumann single-core CPU
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

int pc = 0, ir = 0, fr = 0;  //系统寄存器
int gr[9] = {0};             //通用寄存器
Instruction cache;           //高速缓存器
int dataSegment[1024] = {0}; //数据段(每个数代表2Byte)
int codeSegment[512] = {0};  //代码段(每个数代表4Byte)

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

int main(void)
{
    LinkInstruction code = initInstruction();
    FILE *codeFile = fopen("dict.dic", "r");
    loadCode(code, codeFile);
    runCode(code);
    fclose(codeFile);
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
void loadCode(LinkInstruction code, FILE *fp)
{
    LinkInstruction node, link = code;
    char tmp[34] = {0}, c = 0;
    int i = 0;
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
        strcpy(node->bin_instruction, tmp);

        codeSegment[i++] = B2D_trasition(tmp, 0, 31, 0);
        link->next = node;
        link = link->next;
        fgets(tmp, 34, fp);
    }
}

//执行指令
void runCode(LinkInstruction code)
{
    int i;
    LinkInstruction cur = code->next;
    while (cur)
    {
        cache = *cur;                    //载入高速缓存器cache
        ir = cache.dec_instruction_high; //载入指令寄存器ir
        switch (cache.op)
        {
        case 0:
            stop();
            break;
        case 1:
            mov();
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            arithmetic_operation();
            break;
        case 6:
        case 7:
        case 8:
            logic_operation();
            break;
        case 9:
            cmp();
            break;
        case 10:
            jmp();
            break;
        case 11:
        case 12:
            standard_io();
            break;

        default:
            break;
        }
        printStatus();
        if (!cur->next) //指令执行顺序控制
            break;
        if (cur->next->address == pc)
        {
            cur = cur->next;
        }
        else
        {
            cur = code->next;
            while (cur->address != pc)
                cur = cur->next;
        }
    }
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
void printStatus()
{
    printf("ip = %d\n", pc);
    printf("flag = %d\n", fr);
    printf("ir = %d\n", ir);
    printf("ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n", gr[1], gr[2], gr[3], gr[4]);
    printf("ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n", gr[5], gr[6], gr[7], gr[8]);
}

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