/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: basic operation of the Matrix class
 */

#include "Matrix.h"

int main(void)
{
    //directly define
    int rows, lines;
    cout << "Please enter the number of rows and line of matrices:" << endl;
    cin >> rows >> lines;
    Matrix A1(rows, lines);
    Matrix A2(rows, lines);
    Matrix A3(rows, lines);

    cout << "Please enter the elements of Matrix A1:" << endl;
    A1.inputMatrix();
    cout << "Please enter the elements of Matrix A2:" << endl;
    A2.inputMatrix();
    system("pause");

    A3 = A1.addMatrix(A2);
    cout << "A3 = A1 + A2" << endl;
    A3.outputMatrix();
    system("pause");
    A3 = A1.subMatrix(A2);
    cout << "A3 = A1 - A2" << endl;
    A3.outputMatrix();
    system("pause");

    //use "new" to define
    Matrix *pA1 = new Matrix(A1);
    Matrix *pA2 = new Matrix(A2);
    Matrix *pA3 = new Matrix(A3);

    *pA3 = pA1->addMatrix(*pA2);
    cout << "pA3 = pA1 + pA2" << endl;
    pA3->outputMatrix();
    system("pause");
    *pA3 = pA1->subMatrix(*pA2);
    cout << "pA3 = pA1 - pA2" << endl;
    pA3->outputMatrix();

    delete pA1;
    delete pA2;
    delete pA3;

    return 0;
}
