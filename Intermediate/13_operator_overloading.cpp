// Intermediate/13_operator_overloading.cpp
#include <iostream>

class Point {
public:
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    Point operator+(const Point& p) {
        return Point(x + p.x, y + p.y);
    }
};

int main() {
    Point p1(1, 2), p2(3, 4);
    Point p3 = p1 + p2;
    std::cout << "Point: (" << p3.x << ", " << p3.y << ")" << std::endl;
    return 0;
}