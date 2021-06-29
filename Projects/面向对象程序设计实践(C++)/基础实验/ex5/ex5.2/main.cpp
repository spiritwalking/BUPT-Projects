/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: overload the operators of class Point
 */

#include <iostream>
#include "Point.h"

int main()
{
    Point p(8, 8);

    cout << "Test 1:" << endl;
    cout << "p = " << p << endl;
    cout << "++p = " << ++p << endl;
    cout << "p = " << p << endl;
    system("pause");

    cout << "Test 2:" << endl;
    cout << "p = " << p << endl;
    cout << "p++ = " << p++ << endl;
    cout << "p = " << p << endl;
    system("pause");

    cout << "Test 3:" << endl;
    cout << "p = " << p << endl;
    cout << "--p = " << --p << endl;
    cout << "p = " << p << endl;
    system("pause");

    cout << "Test 4:" << endl;
    cout << "p = " << p << endl;
    cout << "p-- = " << p-- << endl;
    cout << "p = " << p << endl;
    system("pause");
}
