/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: OOP experiment: eTrade system
 */

#include "System.h"

int main()
{
    cout << "Enter \"help\" for more information." << endl;

    System sys("", "");
    sys.exeCommand(std::string("127.0.0.1"), std::string("8082"));
    return 0;
}