#include <iostream>
#include <cmath>

using namespace std;

class Point
{
private:
    double x, y;

public:
    Point(double a = 0, double b = 0);
    ~Point();
    void setPoint(double a = 0, double b = 0);
    double getDistance(Point p);
    void print();
};

Point::Point(double a,double b)
{
    x = a;
    y = b;
    cout << "The point (" << x << ',' << y << ") has been created." << endl;
}

Point::~Point()
{
    cout << "Destruction of class: Point." << endl;
}

void Point::setPoint(double a, double b)
{
    x = a;
    y = b;
}

double Point::getDistance(Point p)
{
    double distance = 0;
    distance = sqrt((x - p.x) * (x - p.x) + (y - p.y) *(y - p.y));
    return distance;
}

void Point::print()
{
    cout << '(' << x << ',' << y << ')';
}