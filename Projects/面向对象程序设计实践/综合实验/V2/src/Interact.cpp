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

void Interact::loadCartFile(string file)
{
    ifstream f(file, ios::in);
    if (!f.good())
    {
        cout << file << ": not exist" << endl;
        return;
    }

    string s, userName;
    int q;
    f >> s;
    while (!f.eof())
    {
        if (haveUser(s))
        {
            userName = s;
            f >> s;
        }
        f >> q;
        addCart(userName, s, q);
        f >> s;
    }
}

void Interact::saveCartFile(string file) const
{
    ofstream f(file, ios::out);
    for (auto uit : userList)
    {
        if (uit->getUserType() == CONSUMER)
        {
            auto conIt = dynamic_cast<Consumer *>(uit);
            if (conIt->cart.empty())
                break;

            f << uit->getName() << endl;
            for (auto carIt : dynamic_cast<Consumer *>(uit)->cart)
                f << carIt.first << " " << carIt.second << endl;
        }
    }
}

//用户管理
bool Interact::haveUser(string username) const
{
    for (auto it : userList)
        if (username == it->getName())
            return true;

    return false;
}

User *Interact::findUser(string userName) const
{
    for (auto uit : userList)
        if (userName == uit->getName())
            return uit;

    return nullptr;
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

Commodity *Interact::findComm(string commName) const
{
    for (auto cit : commList)
        if (commName == cit->getName())
            return cit;

    return nullptr;
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

//购物
bool Interact::buy(string userName, string commName, int num)
{
    if (!haveComm(commName))
        return false;

    auto cit = findComm(commName); //判断商品数量是否足够
    if (num > cit->getQuantity())
    {
        cout << "Sorry, inventory shortage ~" << endl;
        return false;
    }
    cit->deQuantity(num);

    double price = num * cit->getPrice();
    string owner = cit->getOwner();
    auto uit = findUser(userName);
    if (uit->getBalance() >= price) //判断余额是否足够
    {
        deBal(userName, price);
        addBal(owner, price);
        cout << "You spend " << price << " buying " << cit->getName() << endl;
        return true;
    }
    else
    {
        cout << "Sorry, you don't have enough balance." << endl;
        return false;
    }
}

bool Interact::addCart(string userName, string commName, int q)
{
    if (!haveComm(commName) || !haveUser(userName) || q < 0)
        return false;

    auto uit = findUser(userName);
    auto cit = findComm(commName);
    if (cit->getQuantity() < q)
    {
        cout << "Sorry, inventory shortage ~" << endl;
        return false;
    }
    auto conIt = dynamic_cast<Consumer *>(uit);
    conIt->addCart(commName, q);
    return true;
}

bool Interact::delCart(string userName, string commName, int q)
{
    if (q < 0)
        return false;

    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    for (auto carIt = conIt->cart.begin(); carIt != conIt->cart.end(); carIt++) //遍历买家购物车
    {
        if (carIt->first == commName) //找到对应商品
        {
            if (carIt->second > q) //减少数目
                conIt->addCart(commName, -1 * q);
            else //删除商品
                conIt->cart.erase(carIt);

            return true;
        }
    }

    return false;
}

void Interact::clearCart(string userName)
{
    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    conIt->cart.erase(conIt->cart.begin(), conIt->cart.end());
}

bool Interact::changeCart(string userName, string commName, int q)
{
    if (q < 0)
        return false;
    
    auto cit = findComm(commName);
    if (cit->getQuantity() < q)
    {
        cout << "Sorry, inventory shortage ~" << endl;
        return false;
    }

    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    for (auto &carIt : conIt->cart)
    {
        if (carIt.first == commName)
        {
            carIt.second = q;
            return true;
        }
    }

    return false;
}

void Interact::listCart(string userName) const
{
    cout.setf(ios::left);
    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    if (conIt->cart.empty())
    {
        cout << "No commodity in your cart." << endl;
        return;
    }

    cout << "=========================================" << endl;
    for (auto carIt : conIt->cart)
    {
        cout << "commName: ";
        cout.width(15);
        cout << carIt.first << "quantity: " << carIt.second << endl;
    }
    cout << "=========================================" << endl;
}

bool Interact::creatOrder(string userName)
{
    for (auto uit : userList) //遍历用户列表
    {
        auto conIt = dynamic_cast<Consumer *>(uit);
        if (uit->getName() == userName && conIt->bill == 0)
        {
            double sum = 0.0;
            int amount = 0;
            for (auto carIt : conIt->cart) //差错检验
            {
                auto cit = findComm(carIt.first);
                amount = cit->getQuantity();
                if (amount < carIt.second)
                {
                    cout << "Sorry, inventory shortage ~" << endl;
                    return false;
                }
            }

            for (auto carIt : conIt->cart) //计算总额,冻结商品
            {
                auto cit = findComm(carIt.first);
                sum += carIt.second * cit->getPrice();
                cit->setQuantity(cit->getQuantity() - carIt.second);
            }

            conIt->order = conIt->cart;
            conIt->bill = sum;
            listOrder(userName);
            return true;
        }
    }
    return false;
}

bool Interact::delOrder(string userName)
{
    for (auto uit : userList)
    {
        auto conIt = dynamic_cast<Consumer *>(uit);
        if (uit->getName() == userName && conIt->bill != 0)
        {
            for (auto carIt : conIt->cart) //解冻商品
            {
                auto cit = findComm(carIt.first);
                cit->setQuantity(cit->getQuantity() + carIt.second);
            }

            conIt->order.erase(conIt->order.begin(), conIt->order.end());
            conIt->bill = 0;
            return true;
        }
    }
    return false;
}

void Interact::listOrder(string userName)
{
    cout.setf(ios::left);
    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    double bill = conIt->bill;
    if (bill == 0)
    {
        cout << "NO order created." << endl;
        return;
    }

    cout << "=========================================" << endl;
    for (auto carIt : conIt->cart)
    {
        cout << "commName: ";
        cout.width(15);
        cout << carIt.first << "quantity: " << carIt.second << endl;
    }
    cout << bill << " in tatal." << endl;
    cout << "=========================================" << endl;
}

bool Interact::payOrder(string userName)
{
    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    if (conIt->bill == 0)
    {
        cout << "Sorry, you don't have any orders." << endl;
        return false;
    }
    double bill = 0;
    for (auto carIt : conIt->cart)
    {
        auto cit = findComm(carIt.first);
        bill += carIt.second * cit->getPrice();
    }
    if (uit->getBalance() < bill) //检查是否有足够余额
    {
        cout << "Sorry, you don't have enough balance." << endl;
        return false;
    }

    for (auto carIt : conIt->cart)
    {
        auto cit = findComm(carIt.first);
        string owner = cit->getOwner();
        deBal(userName, carIt.second * cit->getPrice());
        addBal(owner, carIt.second * cit->getPrice());
    }
    cout << "You have spent " << bill << " paying the order." << endl;
    conIt->bill = 0;
    clearCart(userName);
    return true;
}

Interact::~Interact()
{
    for (auto it : userList)
        delete it;
    for (auto it : commList)
        delete it;
}