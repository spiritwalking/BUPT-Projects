#ifndef _COMMODITY_H
#define _COMMODITY_H

#include <string>

using namespace std;

class Commodity
{
private:
    double price;
    int quantity;
    string type;
    string name;
    string owner;
    string description;

public:
    virtual double getDiscount() const = 0;
    virtual void setDiscount(double d) = 0;

    virtual double getPrice() const = 0;
    double getOriPrice() const { return price; } //获得初始价格
    void setPrice(double p) { price = p; }       //设置价格

    int getQuantity() const { return quantity; } //获得商品数量
    void setQuantity(int q) { quantity = q; }    //设置商品数量
    bool deQuantity(int count);                  //减少商品数量

    string getType() const { return type; } //获得商品类型
    void setType(string t) { type = t; }    //设置类型

    string getName() const { return name; } //获得商品名
    void setName(string n) { name = n; }    //设置商品名

    string getDescription() const { return description; }  //获得商品描述
    void setDescription(string des) { description = des; } //设置描述

    string getOwner() const { return owner; } //获得所有者
    void setOwner(string o) { owner = o; }    //设置所有者

    Commodity(double p, int q, string t, string n, string o, string des);
    virtual ~Commodity() {}
};

class Food : public Commodity
{
private:
    double discount;

public:
    double getPrice() const { return getOriPrice() * discount; } //获得折后价格
    void setDiscount(double d) { discount = d; }                 //设置折扣
    double getDiscount() const { return discount; }              //获得折扣

    Food(double p, double d, int q, string t, string n, string o, string des);
};

class Book : public Commodity
{
private:
    double discount;

public:
    double getPrice() const { return getOriPrice() * discount; } //获得折后价格
    void setDiscount(double d) { discount = d; }
    double getDiscount() const { return discount; }

    Book(double p, double d, int q, string t, string n, string o, string des);
};

class Cloth : public Commodity
{
private:
    double discount;

public:
    double getPrice() const { return getOriPrice() * discount; } //获得折后价格
    void setDiscount(double d) { discount = d; }
    double getDiscount() const { return discount; }

    Cloth(double p, double d, int q, string t, string n, string o, string des);
};

#endif
