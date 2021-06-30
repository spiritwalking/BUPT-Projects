#include "Commodity.h"

Commodity::Commodity(double p, int q, string t, string n, string o, string des)
    : price(p), quantity(q), type(t), name(n), owner(o), description(des)
{
}

bool Commodity::deQuantity(int count)
{
    if (count <= quantity)
    {
        quantity -= count;
        return true;
    }
    return false;
}

Food::Food(double p, double d, int q, string t, string n, string o, string des)
    : Commodity(p, q, t, n, o, des)
{
    discount = d;
}

Book::Book(double p, double d, int q, string t, string n, string o, string des)
    : Commodity(p, q, t, n, o, des)
{
    discount = d;
}

Cloth::Cloth(double p, double d, int q, string t, string n, string o, string des)
    : Commodity(p, q, t, n, o, des)
{
    discount = d;
}
