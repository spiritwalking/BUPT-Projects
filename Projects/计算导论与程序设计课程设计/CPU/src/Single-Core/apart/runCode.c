#include "definition.h"

extern int pc, ir, fr;
extern int gr[9];
extern Instruction cache;
extern int dataSegment[1024];
extern int codeSegment[512];

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

//输出寄存器状态
void printStatus()
{
    printf("ip = %d\n", pc);
    printf("flag = %d\n", fr);
    printf("ir = %d\n", ir);
    printf("ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n", gr[1], gr[2], gr[3], gr[4]);
    printf("ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n", gr[5], gr[6], gr[7], gr[8]);
}