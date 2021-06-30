/*
 * @Author: Chunyu Ye
 * @Student ID: 2019212686
 * @Class Number: 2019211318
 * @Descripttion: basic operation of the Circle class
 */

#include "Circle.h"

const double esp = 1e-6;

int main(void)
{
    double newx, newy, newr;
    Circle circle1, circle2;

    //input Circle 1
    cout << "Please enter the coordinates of the center of Circle 1:" << endl;
    cin >> newx >> newy;
    cout << "Please enter the radius of Circle 1:" << endl;
    cin >> newr;
    if(newr<=0)
    {
        cout << "Radius must be greater than zero!" << endl;
		cout << "Please enter the radius of Circle 1 again:" << endl;
		cin >> newr;
    }
    Point newCenter1(newx, newy);
    circle1.setParameter(newCenter1, newr);
    system("pause");

    //input Circle 2
    cout << "Please enter the coordinates of the center of Circle 2:" << endl;
    cin >> newx >> newy;
    cout << "Please enter the radius of Circle 2:" << endl;
    cin >> newr;
    if(newr<=0)
    {
        cout << "Radius must be greater than zero!" << endl;
		cout << "Please enter the radius of Circle 2 again:" << endl;
		cin >> newr;
    }
    Point newCenter2(newx, newy);
    circle2.setParameter(newCenter2, newr);
    system("pause");

    double sum = circle1.getRadius() + circle2.getRadius();
    double sub = fabs(circle1.getRadius() - circle2.getRadius());
    double distance = circle1.calDistance(circle2);
    //Judge and output the result
    if (distance>sum && fabs(sum-distance)>esp)
	    cout << "Two circles lying outside each other." << endl;
	else if (fabs(distance-sum)<esp)
	    cout << "Two circles touching each other externally." << endl;
	else if (fabs(distance-sub)<esp)
	    cout << "Two circles touching each other internally." << endl;
	else if (distance<sub && fabs(sub-distance)>esp)
	    cout << "One circle lying inside another." << endl;
	else
        cout << "Two circles intersecting each other." << endl;
    system("pause");

    return 0;
}