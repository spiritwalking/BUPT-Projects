#include <iostream>
using namespace std;

class Point
{
public:
	Point(int x = 0, int y = 0);
	~Point();

	Point &operator++();
	Point operator++(int);
	Point &operator--();
	Point operator--(int);
	friend ostream &operator<<(ostream &out, const Point &p);

private:
	int x, y;
};