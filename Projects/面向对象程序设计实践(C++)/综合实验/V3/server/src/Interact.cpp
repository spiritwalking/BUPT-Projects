#include <iostream>
#include <fstream>
#include "Interact.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <sstream>
#include <strstream>

static string cmd[] =
    {
        "help",
        "sign_up",
        "sign_in",
        "list_comm",
        "search_comm",
        "list_user",
        "quit",
        "sign_out",
        "change_pass",
        "charge",
        "buy",
        "list_cart",
        "add_cart",
        "delete_cart",
        "change_cart",
        "list_order",
        "create_order",
        "delete_order",
        "pay_order",
        "add_comm",
        "change_quantity",
        "change_price",
        "change_discount",
};

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

string Interact::getUserInfo(string username) const
{
    string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    ostrstream oss(buff, MAXBUF);
    for (auto it : userList)
    {
        if (username == it->getName())
        {
            oss << "========User Info=======" << endl;
            // 输出用户信息
            oss << "Name: " << it->getName() << endl;

            oss << "Type: ";
            if (it->getUserType() == CONSUMER)
                oss << "CONSUMER" << endl;
            else
                oss << "MERCHANT" << endl;
            oss << "Balance: " << it->getBalance() << endl;
            oss << "========================" << endl;
        }
    }
    ret = buff;
    if (ret.size() == 0)
    {
        oss << "CANNOT find user : " << username << endl;
        ret = buff;
    }
    return ret;
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

string Interact::listComm(string name) const
{
    string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    ostrstream oss(buff, MAXBUF);
    for (int i = 0; i < 130; i++) //美化界面
        oss << "=";
    oss << endl;

    for (auto it : commList)
    {
        if (name == "*" || it->getName().find(name) != string::npos)
        {
            oss << "Name: " << it->getName() << "\t"
                << "Price: " << it->getPrice() << "\t"
                << "Type: " << it->getType() << "\t"
                << "Quantity: " << it->getQuantity() << "\t"
                << "Discount: " << it->getDiscount() << "\t"
                << "Owner: " << it->getOwner() << "\t"
                << "Description: " << it->getDescription() << endl;
        }
    }
    for (int i = 0; i < 130; i++)
        oss << "=";
    oss << endl;
    ret = buff;
    return ret;
}

bool Interact::addComm(double p, double d, int q, string t, string n, string o, string des)
{
    if (p < 0 || haveComm(n))
    {
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
        return false;

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
string Interact::buy(string userName, string commName, int num)
{
    string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    ostrstream oss(buff, MAXBUF);
    if (!haveComm(commName))
    {
        oss << "sorry, we don't have this commodity";
        ret = buff;
        return ret;
    }

    auto cit = findComm(commName); //判断商品数量是否足够
    if (num > cit->getQuantity())
    {
        oss << "Sorry, inventory shortage ~" << endl;
        ret = buff;
        return ret;
    }
    cit->deQuantity(num);

    double price = num * cit->getPrice();
    string owner = cit->getOwner();
    auto uit = findUser(userName);
    if (uit->getBalance() >= price) //判断余额是否足够
    {
        deBal(userName, price);
        addBal(owner, price);
        oss << "You spend " << price << " buying " << cit->getName() << endl;
        ret = buff;
        return ret;
    }
    else
    {
        oss << "Sorry, you don't have enough balance." << endl;
        ret = buff;
        return ret;
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

string Interact::listCart(string userName) const
{
    string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    ostrstream oss(buff, MAXBUF);

    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    if (conIt->cart.empty())
    {
        oss << "No commodity in your cart." << endl;
        ret = buff;
        return ret;
    }

    oss << "=========================================" << endl;
    for (auto carIt : conIt->cart)
    {
        oss << "commName: ";
        oss.width(15);
        oss << carIt.first << "quantity: " << carIt.second << endl;
    }
    oss << "=========================================" << endl;
    ret = buff;
    return ret;
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

string Interact::listOrder(string userName)
{
    string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    ostrstream oss(buff, MAXBUF);

    oss.setf(ios::left);
    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    double bill = conIt->bill;
    if (bill == 0)
    {
        oss << "NO order created." << endl;
        ret = buff;
        return ret;
    }

    oss << "=========================================" << endl;
    for (auto carIt : conIt->cart)
    {
        oss << "commName: ";
        oss.width(15);
        oss << carIt.first << "quantity: " << carIt.second << endl;
    }
    oss << bill << " in tatal." << endl;
    oss << "=========================================" << endl;
    ret = buff;
    return ret;
}

string Interact::payOrder(string userName)
{
    string ret;
    char buff[MAXBUF];
    memset(buff, 0, MAXBUF);
    ostrstream oss(buff, MAXBUF);

    auto uit = findUser(userName);
    auto conIt = dynamic_cast<Consumer *>(uit);
    if (conIt->bill == 0)
    {
        oss << "Sorry, you don't have any orders." << endl;
        ret = buff;
        return ret;
    }
    double bill = 0;
    for (auto carIt : conIt->cart)
    {
        auto cit = findComm(carIt.first);
        bill += carIt.second * cit->getPrice();
    }
    if (uit->getBalance() < bill) //检查是否有足够余额
    {
        oss << "Sorry, you don't have enough balance." << endl;
        ret = buff;
        return ret;
    }

    for (auto carIt : conIt->cart)
    {
        auto cit = findComm(carIt.first);
        string owner = cit->getOwner();
        deBal(userName, carIt.second * cit->getPrice());
        addBal(owner, carIt.second * cit->getPrice());
    }
    oss << "You have spent " << bill << " paying the order." << endl;
    conIt->bill = 0;
    clearCart(userName);
    ret = buff;
    return ret;
}

Interact::~Interact()
{
    for (auto it : userList)
        delete it;
    for (auto it : commList)
        delete it;
}

Interact::Interact()
{
    loadUserFile();
    loadCartFile();
    loadCommFile();
}

int Interact::exec(const string &port)
{
    // 初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // 生成套接字
    SOCKET serverFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in myAddr;
    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = PF_INET;
    myAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    myAddr.sin_port = htons(atoi(port.c_str()));
    bind(serverFd, (SOCKADDR *)&myAddr, sizeof(myAddr));

    listen(serverFd, 20);
    char buffRecv[MAXBUF];
    char buffSend[MAXBUF];
    unsigned int len = 0;

    SOCKADDR clientAddr;
    int addrLen = sizeof(clientAddr);

    SOCKET clientFd;
    while (1)
    {

        memset(buffRecv, 0, MAXBUF);
        memset(buffSend, 0, MAXBUF);
        memset(&clientAddr, 0, sizeof(clientAddr));
        len = 0;

        clientFd = accept(serverFd, (SOCKADDR *)&clientAddr, &addrLen);

        cout << "Accept a request\n";

        int ret = recv(clientFd, buffRecv, MAXBUF, 0); // 接受客户端消息

        cout << "len : " << ret << endl;

        istringstream iss(buffRecv + 2);

        char oper;
        // iss >> oper;
        oper = buffRecv[0];

        cout << "accept the command :" << cmd[int(oper)] << endl;

        string name, pwd, type, t, cname, des;
        int num;
        double mon;

        switch (oper)
        {
        case 1: // sign_up
        {
            iss >> name >> pwd >> num;
            if (!addUser(name, pwd, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 2: // sign_in
        {
            iss >> name >> pwd;
            int temp_t;
            if (!checkPass(name, pwd, temp_t))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                buffSend[1] = ' ';

                ostrstream oss(buffSend + 2, MAXBUF);

                // buffSend[2] = type;
                oss << temp_t;

                len += MAXBUF;
                break;
            }
            break;
        }

        case 3: // listcomm
        {
            string ret = listComm("*");
            memcpy(buffSend, ret.c_str(), ret.size());
            len += ret.size();
            break;
        }

        case 4: // searchcomm
        {
            iss >> cname;
            string ret = listComm(cname);
            memcpy(buffSend, ret.c_str(), ret.size());
            len += ret.size();
            break;
        }

        case 5: // list_user
        {
            iss >> name;
            {
                string ret = getUserInfo(name);
                memcpy(buffSend, ret.c_str(), ret.size());
                len += ret.size();
            }
            break;
        }

        case 6: // quit
        {
            buffSend[0] = '1';
            saveCommFile();
            saveUserFile();
            saveCartFile();
            break;
        }

        case 7: // sign_out
        {
            break;
        }

        case 8: // change_pass
        {
            iss >> name >> pwd;
            if (!changePass(name, pwd))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 9: // recharge
        {
            iss >> name >> mon;
            if (!addBal(name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 10: // buy
        {
            iss >> name >> cname >> num;
            string ret = buy(name, cname, num);
            memcpy(buffSend, ret.c_str(), ret.size());
            len += ret.size();
            break;
        }

        case 11: // list_cart
        {
            iss >> name;
            string ret = listCart(name);
            memcpy(buffSend, ret.c_str(), ret.size());
            len += ret.size();
            break;
        }

        case 12: // add_cart
        {
            iss >> name >> cname >> num;
            if (!haveComm(cname))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            if (!addCart(name, cname, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 13: // delete_cart
        {
            iss >> name >> cname >> num;
            if (!haveComm(cname))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            if (!delCart(name, cname, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 14: // change_cart
        {
            iss >> name >> cname >> num;
            if (!haveComm(cname))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            if (!changeCart(name, cname, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 15: // list_order
        {
            iss >> name;
            string ret = listOrder(name);
            memcpy(buffSend, ret.c_str(), ret.size());
            len += ret.size();
            break;
        }

        case 16: // create_order
        {
            iss >> name;
            if (!creatOrder(name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 17: // delete_order
        {
            iss >> name;
            if (!delOrder(name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 18: // pay_order
        {
            iss >> name;
            string ret = payOrder(name);
            memcpy(buffSend, ret.c_str(), ret.size());
            len += ret.size();
            break;
        }

        case 19: // add_comm
        {
            iss >> mon >> num >> type >> cname >> name >> des;
            if (!addComm(mon, 1.0, num, type, cname, name, des))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 20: // change_quantity
        {
            iss >> cname >> name >> num;
            if (!setQuantity(cname, name, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 21: // change_price
        {
            iss >> cname >> name >> mon;
            if (!setPrice(cname, name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }

        case 22: // change_discount
        {
            iss >> mon >> t >> name;
            if (!setDiscount(mon, t, name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;
        }
        }
        send(clientFd, buffSend, len, 0);
        saveCommFile();
        saveUserFile();
        saveCartFile();
        close(clientFd);
    }
}