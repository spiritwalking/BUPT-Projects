#include <math.h>
#include "definition.h"

extern int dataSegment[1024];
extern int codeSegment[512];

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