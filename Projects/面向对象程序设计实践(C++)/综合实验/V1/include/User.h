#ifndef _USER_H
#define _USER_H

#include <string>
#include <vector>
#include <utility>
#include <iostream>

#define VISITOR -1
#define CONSUMER 0
#define MERCHANT 1

using namespace std;

class User
{
private:
    string username;
    string password;
    double balance;

public:
    int type;
    virtual int getUserType() const = 0; //获得用户类型

    double getBalance() const { return balance; } //获得余额
    void setBalance(double b) { balance = b; }    //设置余额

    string getName() const { return username; } //获得用户名
    void setName(string nm) { username = nm; }  //设置用户名

    string getPassword() const { return password; } //获得密码
    void setPassword(string pw) { password = pw; }  //设置密码

    User(string n, string pw, double b); //构造函数
    virtual ~User() {}
};

class Consumer : public User
{
public:
    int getUserType() const { return type; } //获得用户类型
    Consumer(string n, string pw, double b);
};

class Merchant : public User
{
public:
    int getUserType() const { return type; } //获得用户类型
    Merchant(string n, string pw, double b);
};

#endif
