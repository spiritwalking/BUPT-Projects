#ifndef _INTERACT_H
#define _INTERACT_H

#include <string>
#include <vector>
#include <map>
#include "User.h"
#include "Commodity.h"

const int MAXBUF = 2048;
#define BOOK 0
#define FOOD 1
#define CLOTH 2

class Interact
{
private:
    vector<User *> userList;      //用户列表
    vector<Commodity *> commList; //商品列表
    Interact(const Interact &t) = default;

public:
    //文件读写
    void loadUserFile(string file = "userdata.txt");
    void saveUserFile(string file = "userdata.txt") const;
    void loadCommFile(string file = "commdata.txt");
    void saveCommFile(string file = "commdata.txt") const;
    void loadCartFile(string file = "cartdata.txt");
    void saveCartFile(string file = "cartdata.txt") const;

    //用户管理
    bool haveUser(string userName) const;                                  //查找是否存在用户
    User *findUser(string userName) const;                                 //返回用户指针
    bool checkPass(string userName, string password, int &t) const;        //校验密码
    bool changePass(string userName, string password);                     //更改密码
    bool addUser(string userName, string password, int type, int bal = 0); //增加用户
    string getUserInfo(string userName) const;                             //获得用户信息
    bool addBal(string userName, double b);                                //增加余额
    bool deBal(string userName, double b);                                 //减少余额

    //商品管理
    bool haveComm(string commName) const;                                              //查找是否存在商品
    Commodity *findComm(string commName) const;                                        //返回商品指针
    string listComm(string commName = "*") const;                                      //列出商品信息
    bool addComm(double p, double d, int q, string n, string o, string t, string des); //添加新商品
    bool delComm(string commName, string userName);                                    //删除商品
    bool setQuantity(string commName, string userName, int q);                         //更改商品数量
    bool setPrice(string commName, string userName, double p);                         //设置价格
    bool setDiscount(double p, string type, string userName);                          //设置折扣

    //购物
    string buy(string userName, string commName, int num);
    bool addCart(string userName, string commName, int q);
    bool delCart(string userName, string commName, int q = 0);
    bool changeCart(string userName, string commName, int q);
    string listCart(string userName) const;
    void clearCart(string userName);
    string payOrder(string userName);
    bool creatOrder(string userName);
    string listOrder(string userName);
    bool delOrder(string userName);
    int exec(const string &port);
    Interact();
    ~Interact();
};

#endif
