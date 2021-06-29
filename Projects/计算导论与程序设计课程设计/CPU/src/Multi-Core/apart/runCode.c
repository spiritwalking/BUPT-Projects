#include "definition.h"

extern int pc[2], ir[2], fr[2];
extern int gr[2][9];
extern int dataSegment[DATAMAX];
extern int codeSegment[CODEMAX];
extern HANDLE hMutex1, hMutex2;

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