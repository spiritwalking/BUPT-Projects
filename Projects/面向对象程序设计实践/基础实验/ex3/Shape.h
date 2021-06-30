#include <iostream>
#define PI 3.1415926

using namespace std;

class Shape
{
public:
	Shape();
	~Shape();

	double getArea();
};

class Circle : public Shape
{
public:
	Circle(double r = 0);
	~Circle();

	void setRadius(double r);
	double getArea();

private:
	double radius;
};

class Rectangle : public Shape
{
public:
	Rectangle(double len = 0, double wid = 0);
	~Rectangle();

	void setConfig(double len, double wid);
	double getArea();

private:
	double length, width;
};

class Square : public Rectangle
{
public:
	Square(double len = 0);
	~Square();
};
