#include "Point.h"

class Circle
{
private:
    Point center;
    double radius;

public:
    Circle();
    ~Circle();
    void setParameter(Point c, double r);
    double getRadius();
    double calDistance(Circle circle2);
};

Circle::Circle()
{
    cout << "Creation of class: Circle." << endl;
}

Circle::~Circle()
{
    cout << "Destruction of class: Circle." << endl;
}

void Circle::setParameter(Point c,double r)
{
    center = c;
    radius = r;
    cout << "A circle with the center at ";
    c.print();
    cout << " and the radius of " << radius << " has been created." << endl;
}

double Circle::getRadius()
{
    return radius;
}

double Circle::calDistance(Circle circle2)
{
    return center.getDistance(circle2.center);
}