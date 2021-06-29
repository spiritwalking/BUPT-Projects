#include "Point.h"

Point::Point(int x, int y) : x(x), y(y)
{
	cout << "Constructor: Point ("
		 << x << "," << y << ")" << endl;
}

Point::~Point()
{
	cout << "Distructor." << endl;
}

Point &Point::operator++()
{
	this->x++;
	this->y++;
	return *this;
}

//int is used to distinguish prefix and suffix
Point Point::operator++(int)
{
	Point temp = *this;
	this->x++;
	this->y++;
	return temp;
}

Point &Point::operator--()
{
	this->x--;
	this->y--;
	return *this;
}

Point Point::operator--(int)
{
	Point temp = *this; //copy constructor
	this->x--;
	this->y--;
	return temp;
}

ostream &operator<<(ostream &out, const Point &p)
{
	out << "(" << p.x << "," << p.y << ")";
	return out;
}
