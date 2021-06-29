/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: basic operation on matrix
 */

#include <iostream>
#define ROW 4
#define COLUMN 5

using namespace std;

int **creat();
void init(int **p);
void addition(int **a, int **b, int **c);
void subtraction(int **a, int **b, int **c);
void print(int **p);
void destroy(int **p);

int main(void)
{
    int **a, **b, **c;
    a = creat();
    b = creat();
    c = creat();

    cout << "please enter Matrix A(4x5):" << endl;
    init(a);
    cout << "please enter Matrix B(4x5):" << endl;
    init(b);

    addition(a, b, c);
    cout << "The result of A + B is:" << endl;
    print(c);
    subtraction(a, b, c);
    cout << "The result of A - B is:" << endl;
    print(c);

    destroy(a);
    destroy(b);
    destroy(c);
    return 0;
}

//Matrix Creation
int **creat()
{
    int **p = new int*[ROW];
    if(!p)
    {
        cout << "Error: out of memory." <<endl;
        exit(1);
    }
    for (int i = 0; i < COLUMN;i++)
    {
        p[i] = new int[COLUMN];
        if(!p[i])
        {
            cout << "Error: out of memory." <<endl;
            exit(1);
        }
    }

    cout << "Matrix creation completed." << endl;
    return p;
}

//Marix initialization
void init(int **p)
{
    for (int i = 0; i < ROW;i++)
    {
        for (int j = 0; j < COLUMN;j++)
        {
            cin >> p[i][j];
        }
    }
}

//Matrix addition
void addition(int **a, int **b, int **c)
{
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			c[i][j] = a[i][j] + b[i][j];   
		}
	}
}

//Matrix subtraction
void subtraction(int **a, int **b, int **c)
{
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			c[i][j] = a[i][j] - b[i][j];   
		}
	}
}

//Matrix output
void print(int **p)
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN-1;j++)
        {
            cout << p[i][j] << "\t";
        }
        cout << p[i][j] << endl;
    }
}

//Matrix destruction
void destroy(int **p)
{
    for (int i = 0; i < COLUMN;i++)
    {
        delete[] p[i];
    }
    delete[] p;
    cout << "Matrix destruction completed." << endl;
}
