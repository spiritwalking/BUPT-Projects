/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: Simulate Von Neumann multi-core CPU
 */

#include "definition.h"

int pc[2] = {0}, ir[2] = {0}, fr[2] = {0}; //系统寄存器
int gr[2][9] = {0};                        //通用寄存器
int dataSegment[DATAMAX] = {0};            //数据段(每个数代表2Byte)
int codeSegment[CODEMAX] = {0};            //代码段(每个数代表4Byte)
HANDLE hMutex1, hMutex2;                   //互斥对象句柄

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