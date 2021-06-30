#include "Shape.h"

Shape::Shape()
{
	cout << "Constructor of Class: Shape." << endl;
}

Shape::~Shape()
{
	cout << "Destructor of Class: Shape." << endl;
}

double Shape::getArea()
{
	return 0;
}

Circle::Circle(double r) : radius(r)
{
	cout << "Constructor of Class: Circle with radius:" << radius << endl;
}

Circle::~Circle()
{
	cout << "Destructor of Class: Circle." << endl;
}

void Circle::setRadius(double r)
{
	radius = r;
	cout << "The radius of the circle is " << radius << endl;
}

double Circle::getArea()
{
	return PI * radius * radius;
}

Rectangle::Rectangle(double len, double wid) : length(len), width(wid)
{
	cout << "Constructor of Class: Rectangle with length:"
		 << length << " width:" << width << endl;
}

Rectangle::~Rectangle()
{
	cout << "Destructor of Class: Rectangle." << endl;
}

void Rectangle::setConfig(double len, double wid)
{
	length = len;
	width = wid;
	cout << "The length of the rectangle is " << length
		 << " and the width is " << width << endl;
}

double Rectangle::getArea()
{
	return length * width;
}

Square::Square(double len) : Rectangle(len, len)
{
	cout << "Constructor of Class: Square with length:" << len << endl;
}

Square::~Square()
{
	cout << "Destructor of Class: Square." << endl;
}