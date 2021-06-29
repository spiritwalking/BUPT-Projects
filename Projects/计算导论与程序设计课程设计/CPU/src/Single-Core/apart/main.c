/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: Simulate Von Neumann single-core CPU
 */

#include "definition.h"

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
