/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: Derived classes based on class Shape with virtual function
 */

#include "Shape.h"

int main()
{
	Shape *s = new Shape();
	cout << "The area of shape is " << s->getArea() << endl;
	system("pause");

	Circle *c = new Circle(4);
	cout << "The area of circle is " << c->getArea() << endl;
	system("pause");

	Rectangle *rec = new Rectangle(8, 16);
	cout << "The area of rectangle is " << rec->getArea() << endl;
	system("pause");

	Square *sq = new Square(32);
	cout << "The area of square is " << sq->getArea() << endl;
	system("pause");

	delete s;
	delete c;
	delete rec;
	delete sq;
}
