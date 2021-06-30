#ifndef _System_H
#define _System_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#define VISITOR -1
#define CONSUMER 0
#define MERCHANT 1
const int MAXBUF = 2048;
using namespace std;

enum cmdVal
{
    //系统命令
    help,
    sign_up,
    sign_in,
    list_comm,
    search_comm,
    list_user,
    quit,
    sign_out,
    change_pass,
    //买家命令
    charge,
    buy,
    list_cart,
    add_cart,
    delete_cart,
    change_cart,
    list_order,
    create_order,
    delete_order,
    pay_order,
    //卖家命令
    add_comm,
    change_quantity,
    change_price,
    change_discount,
};

class System
{
private:
    string username; //当前用户名
    string password; //当前用户密码
    int type;        //当前用户类型
    bool logged;     //是否登录

public:
    int getType() const { return type; }             //获得当前用户类型
    string getName() const { return username; }      //获得当前用户名
    void setName(string n) { username = n; }         //设置当前用户名
    string getPassword() const { return password; }  //获得当前用户密码
    void setPassword(string pwd) { password = pwd; } //设置密码
    int exeCommand();                                //执行命令

    System(string un, string pwd);
};

#endif
