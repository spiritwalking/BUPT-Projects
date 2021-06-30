/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: Derived classes based on abstract class Shape
 */

#include "Shape.h"

int main()
{
	//Shape s;
	cout << "Can not instantiate abstract class: Shape." << endl;
	system("pause");

	Shape *c = new Circle(4);
	cout << "The area of circle is " << c->getArea() << endl;
	system("pause");

	Shape *rec = new Rectangle(8, 16);
	cout << "The area of rectangle is " << rec->getArea() << endl;
	system("pause");

	Shape *sq = new Square(32);
	cout << "The area of square is " << sq->getArea() << endl;
	system("pause");
	
	delete c;
	delete rec;
	delete sq;
}
