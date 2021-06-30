#include "User.h"

User::User(string n, string pw, double b)
{
    username = n;
    password = pw;
    balance = b;
    type = VISITOR;
}

Consumer::Consumer(string n, string pw, double b) : User(n, pw, b)
{
    type = CONSUMER;
}

Merchant::Merchant(string n, string pw, double b) : User(n, pw, b)
{
    type = MERCHANT;
}
