#include "User.h"

User::User(string n, string pw, double b)
{
    username = n;
    password = pw;
    balance = b;
    type = VISITOR;
}

void Consumer::addCart(string commName, int amount)
{
    for (auto &it : cart)
    {
        if (commName == it.first) //购物车已有此商品
        {
            it.second += amount;
            return;
        }
    }

    pair<string, int> p(commName, amount); //新建商品清单表项
    cart.push_back(p);                     //添加商品清单
}

Consumer::Consumer(string n, string pw, double b) : User(n, pw, b)
{
    type = CONSUMER;
}

Merchant::Merchant(string n, string pw, double b) : User(n, pw, b)
{
    type = MERCHANT;
}
