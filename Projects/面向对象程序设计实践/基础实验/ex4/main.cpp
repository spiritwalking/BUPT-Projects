/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: Experiment on io stream
 */

#include <iostream>
#include <limits>
#include <ctime>
#include <cstdlib>
#define MIN 1
#define MAX 1000
using namespace std;

int main()
{
    int x, price;

    srand(unsigned(time(NULL)));
    price = rand() % 1000 + 1;

    cout << "Please enter an integer x (1 <= x <= 1000) :" << endl;
    while (true)
    {
        cin >> x;

        // x is illegal
        try
        {
            if (x < MIN || x > MAX || cin.fail())
            {
                throw "Your input is illegal!";
            }
        }
        catch (const char *msg)
        {
            cin.clear(); //reset failbit
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << msg << endl;
            continue;
        }

        // x in range
        if (x == price)
        {
            cout << "Your guess is right!" << endl;
            break;
        }
        else if (x < price)
        {
            cout << "Your guess (" << x << ") is smaller than the right price." << endl;
        }
        else if (x > price)
        {
            cout << "Your price (" << x << ") is bigger than the right price." << endl;
        }
    }

    cout << "Game over." << endl;
    return 0;
}
