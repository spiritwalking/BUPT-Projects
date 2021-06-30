#include <iostream>
#include <fstream>
#include "Interact.h"

//文件读写
void Interact::loadUserFile(string file)
{
    ifstream f(file, ios::in);
    if (!f.good())
    {
        cout << file << ": not exist" << endl;
        return;
    }

    string name, pwd;
    int type;
    double bal;
    f >> name;
    while (!f.eof())
    {
        f >> pwd >> type >> bal;
        addUser(name, pwd, type, bal);

        f >> name;
    }
    f.close();
}

void Interact::saveUserFile(string file) const
{
    ofstream f(file, ios::out);
    for (auto it : userList)
    {
        f << it->getName() << " " << it->getPassword() << " "
          << it->getUserType() << " " << it->getBalance() << endl;
    }
    f.close();
}

void Interact::loadCommFile(string file)
{
    ifstream f(file, ios::in);
    if (!f.good())
    {
        cout << file << ": not exist" << endl;
        return;
    }

    string name, type, owner, des;
    double price, discount;
    int quantity;
    f >> name;
    while (!f.eof())
    {
        f >> type >> owner >> price >> discount >> quantity >> des;
        addComm(price, discount, quantity, type, name, owner, des);

        f >> name;
    }
    f.close();
}

void Interact::saveCommFile(string file) const
{
    ofstream f(file, ios::out);
    for (auto it : commList)
    {
        f << it->getName()
          << " " << it->getType()
          << " " << it->getOwner()
          << " " << it->getOriPrice()
          << " " << it->getDiscount()
          << " " << it->getQuantity()
          << " " << it->getDescription() << endl;
    }
    f.close();
}

//用户管理
bool Interact::haveUser(string username) const
{
    for (auto it : userList)
        if (username == it->getName())
            return true;

    return false;
}

bool Interact::checkPass(string username, string password, int &t) const
{
    for (auto it : userList)
    {
        if (username == it->getName() && password == it->getPassword())
        {
            t = it->type;
            return true;
        }
    }
    return false;
}

bool Interact::addUser(string username, string password, int type, int bal)
{
    if (haveUser(username))
    {
        cout << "Error: Repeated user." << endl;
        return false;
    }

    User *u = nullptr;
    if (type == CONSUMER)
        u = new Consumer(username, password, bal);
    else if (type == MERCHANT)
        u = new Merchant(username, password, bal);
    else
        cout << "Error: No user type: " << type << endl;

    if (u != nullptr)
        userList.push_back(u);
    return true;
}

bool Interact::getUserInfo(string username) const
{
    for (auto it : userList)
    {
        if (username == it->getName())
        {
            cout << "========User Info=======" << endl;
            // 输出用户信息
            cout << "Name: " << it->getName() << endl;

            cout << "Type: ";
            if (it->getUserType() == CONSUMER)
                cout << "CONSUMER" << endl;
            else
                cout << "MERCHANT" << endl;
            cout << "Balance: " << it->getBalance() << endl;
            cout << "========================" << endl;
            return true;
        }
    }

    cout << "CANNOT find user : " << username << endl;
    return false;
}

bool Interact::addBal(string username, double b)
{
    if (b < 0)
        return false;
    for (auto uit : userList)
    {
        if (username == uit->getName())
        {
            uit->setBalance(uit->getBalance() + b);
            return true;
        }
    }
    return false;
}

bool Interact::deBal(string username, double b)
{
    for (auto uit : userList)
        if (username == uit->getName())
            uit->setBalance(uit->getBalance() - b);

    return false;
}

bool Interact::changePass(string username, string password)
{
    for (auto uit : userList)
    {
        if (username == uit->getName())
        {
            uit->setPassword(password);
            return true;
        }
    }
    return false;
}

//商品管理
bool Interact::haveComm(string name) const
{
    for (auto it : commList)
        if (name == it->getName())
            return true;

    return false;
}

bool Interact::listComm(string name) const
{
    for (int i = 0; i < 130; i++) //美化界面
        cout << "=";
    cout << endl;

    bool flag = false;
    for (auto it : commList)
    {
        if (name == "*" || it->getName().find(name) != string::npos)
        {
            flag = true;
            cout << "Name: " << it->getName() << "\t"
                 << "Price: " << it->getPrice() << "\t"
                 << "Type: " << it->getType() << "\t"
                 << "Quantity: " << it->getQuantity() << "\t"
                 << "Discount: " << it->getDiscount() << "\t"
                 << "Owner: " << it->getOwner() << "\t"
                 << "Description: " << it->getDescription() << endl;
        }
    }
    for (int i = 0; i < 130; i++)
        cout << "=";
    cout << endl;
    return flag;
}

bool Interact::addComm(double p, double d, int q, string t, string n, string o, string des)
{
    if (p < 0 || haveComm(n))
    {
        cout << "Error: Add commodity." << endl;
        return false;
    }

    Commodity *c = nullptr;
    if (t == "book")
        c = new Book(p, d, q, t, n, o, des);
    else if (t == "food")
        c = new Food(p, d, q, t, n, o, des);
    else if (t == "cloth")
        c = new Cloth(p, d, q, t, n, o, des);
    else
        cout << "Error: No commodity type: " << t << endl;

    if (c != nullptr)
        commList.push_back(c);
    return true;
}

bool Interact::delComm(string commName, string userName)
{
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (commName == (*it)->getName() && userName == (*it)->getOwner())
        {
            commList.erase(it);
            return true;
        }
    }
    return false;
}

bool Interact::setQuantity(string commName, string userName, int q)
{
    if (q < 0)
        return false;

    for (auto it : commList)
    {
        if (commName == it->getName() && userName == it->getOwner())
        {
            it->setQuantity(q);
            return true;
        }
    }
    return false;
}

bool Interact::setPrice(string commName, string userName, double p)
{
    if (p < 0)
        return false;

    for (auto it : commList)
    {
        if (commName == it->getName() && userName == it->getOwner())
        {
            it->setPrice(p);
            return true;
        }
    }
    return false;
}

bool Interact::setDiscount(double d, string type, string username)
{
    bool flag = false;
    if (d < 0)
    {
        return false;
    }
    for (auto it : commList)
    {
        if (type == it->getType() && username == it->getOwner())
        {
            it->setDiscount(d);
            flag = true;
        }
    }

    return flag;
}

bool Interact::buy(string userName, string commName, int num)
{
    if (!haveComm(commName))
        return false;

    auto cit = commList.begin();
    while (cit != commList.end())
    {
        if (commName == (*cit)->getName())
        {
            if (num > (*cit)->getQuantity())
            {
                cout << "Sorry, inventory shortage ~" << endl;
                return false;
            }
            (*cit)->deQuantity(num);
            break;
        }
        cit++;
    }

    double price = num * (*cit)->getPrice();
    string owner = (*cit)->getOwner();
    for (auto it : userList)
    {
        if (userName == it->getName())
        {
            if (it->getBalance() >= price)
            {
                deBal(userName, price);
                addBal(owner, price);
                cout << "You spend " << price << " buying " << (*cit)->getName() << endl;
                return true;
            }
            else
            {
                cout << "Sorry, you don't have enough balance." << endl;
                return false;
            }
        }
    }
    return false;
}

Interact::~Interact()
{
    for (auto it : userList)
        delete it;
    for (auto it : commList)
        delete it;
}