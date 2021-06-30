#include <iostream>
using namespace std;

class Matrix
{
private:
    int rows, lines;
    int **head;

public:
    Matrix(int r, int l);      //Constructor
    Matrix(const Matrix &obj); //Copy Constructor
    ~Matrix();                 //Destructor

    void inputMatrix();
    void outputMatrix();
    Matrix addMatrix(Matrix m);
    Matrix subMatrix(Matrix m);
    Matrix &operator=(const Matrix &m);
};

Matrix::Matrix(int r, int l)
{
    rows = r;
    lines = l;
    head = new int *[rows];
    for (int i = 0; i < rows; i++)
    {
        head[i] = new int[lines];
    }
    cout << "A " << rows << "x" << lines
         << " matrix has been created." << endl;
}

//Copy Constructor
Matrix::Matrix(const Matrix &m)
{
    rows = m.rows;
    lines = m.lines;
    head = new int *[rows];
    for (int i = 0; i < rows; i++)
    {
        head[i] = new int[lines];
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < lines; j++)
        {
            head[i][j] = m.head[i][j];
        }
    }

    //cout << "copy constructor." << endl;
}

Matrix::~Matrix()
{
    if (head)
    {
        for (int i = 0; i < rows; i++)
        {
            delete[] head[i];
        }
        delete[] head;
    }
    //cout << "The matrix has been destroyed." << endl;
}

void Matrix::inputMatrix()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < lines; j++)
        {
            cin >> head[i][j];
        }
    }
    cout << "Input completed." << endl;
}

void Matrix::outputMatrix()
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < lines - 1; j++)
        {
            cout << head[i][j] << "\t";
        }
        cout << head[i][j] << endl;
    }
}

Matrix Matrix::addMatrix(Matrix m)
{
    Matrix tmp(rows, lines);
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < lines; j++)
        {
            tmp.head[i][j] = head[i][j] + m.head[i][j];
        }
    }
    cout << "Addtion completed." << endl;
    return tmp;
}

Matrix Matrix::subMatrix(Matrix m)
{
    Matrix tmp(rows, lines);
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < lines; j++)
        {
            tmp.head[i][j] = head[i][j] - m.head[i][j];
        }
    }
    cout << "Subtraction completed." << endl;
    return tmp;
}

Matrix &Matrix::operator=(const Matrix &m)
{
    //avoid self-assignment
    if (&m != this)
    {
        //delete old elements
        for (int i = 0; i < rows && head != NULL; i++)
        {
            delete[] head[i];
        }
        delete[] head;

        //value assignment
        head = new int *[rows];
        for (int i = 0; i < rows; i++)
        {
            head[i] = new int[lines];
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < lines; j++)
            {
                head[i][j] = m.head[i][j];
            }
        }
    }
    cout << "use overloaded operator \"=\"." << endl;
    return *this;
}
