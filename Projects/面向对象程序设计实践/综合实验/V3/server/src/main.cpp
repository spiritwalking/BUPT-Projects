#include "Interact.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    cout << "Server V3\n"
              << "Usage : V3-server [port]\n";
    Interact trade;
    trade.exec(string("8082"));
}
