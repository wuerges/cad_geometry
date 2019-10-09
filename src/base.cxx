#include <geometry.hpp>
#include <iostream>
#include <array>
#include <cassert>

namespace geometry {

std::ostream & operator<<(std::ostream & out, const Shape & s) {
    out << "Shape{"<<s.a<<", "<<s.b<<"}";
    // out << "Polygon(";
    // out << "(" << s.a.x << "," << s.a.y << "),";
    // out << "(" << s.b.x << "," << s.a.y << "),";
    // out << "(" << s.b.x << "," << s.b.y << "),";
    // out << "(" << s.a.x << "," << s.b.y << ")";
    // out << ")";
    return out;
}
std::ostream & operator<<(std::ostream & out, const PT & s) {
    out << "PT{"<<s.x<<", "<<s.y<<", "<<s.z<<"}";
    return out;
}


int PT::operator[](int i) const {
    if(i==0) return x;
    if(i==1) return y;
    if(i==2) return z;
    assert(0 && "point index out of bounds");
    return -1;
}

const int manhatan(const PT & a, const PT & b) {
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);;
}

const bool operator<(const Shape & a, const Shape & b) {
    std::array<int, 6> va = {a.a.x, a.a.y, a.a.z, a.b.x, a.b.y, a.b.z };
    std::array<int, 6> vb = {b.a.x, b.a.y, b.a.z, b.b.x, b.b.y, b.b.z };
    return va < vb;
}

const bool operator==(const Shape & a, const Shape & b) {
    return a.a == b.a && a.b == b.b;
}
const bool operator==(const PT & a, const PT & b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

const bool operator!=(const Shape & a, const Shape & b) {
    return ! (a == b);
}

int dist(int a, int b, int aw, int bw) {
    if(a < b) {
        if(a + aw < b) {
            return b - a - aw;
        }
        return 0;
    }
    else {
        if(b + bw < a) {
            return a - b - bw;
        }
        return 0;
    }
}


const int distance(const Shape & s1, const Shape & s2) {
    return dist(s1.a.x, s2.a.x, abs(s1.b.x - s1.a.x),  abs(s2.b.x - s2.a.x))
            + dist(s1.a.y, s2.a.y, abs(s1.b.y - s1.a.y),  abs(s2.b.y - s2.a.y))
            + dist(s1.a.z, s2.a.z, abs(s1.b.z - s1.a.z),  abs(s2.b.z - s2.a.z));
            // + abs(s1.a.z - s2.b.z);
}

const int distance(const PT & pt, const Shape & s2) {
    return distance(Shape(pt, pt), s2);
}

const bool operator<(const PT & a, const PT & b) {
    return a.x < b.x && a.y < b.y && a.z < b.z;
}
const bool operator<=(const PT & a, const PT & b) {
    return a.x <= b.x && a.y <= b.y && a.z <= b.z;
}
const bool operator>(const PT & a, const PT & b) {
    return b < a;
}
const bool operator>=(const PT & a, const PT & b) {
    return b <= a;
}
const PT min(const PT & a, const PT & b) {
    using std::min;
    return PT(
        min(a.x, b.x),
        min(a.y, b.y),
        min(a.z, b.z));
}

const PT max(const PT & a, const PT & b) {
    using std::max;
    return PT(
        max(a.x, b.x),
        max(a.y, b.y),
        max(a.z, b.z));
}

const bool collides(const PT & p, const Shape & s2) {
    return collides(Shape{p,p}, s2);
}

const bool collides(const int ax1, const int ax2, const int bx1, const int bx2) {
    return (ax1 <= bx1 && bx1 <= ax2) 
        || (ax1 <= bx2 && bx2 <= ax2) 
        || (bx1 <= ax1 && ax1 <= bx2);
}

const bool collides(const Shape & s1, const Shape & s2) {
    return collides(s1.a.x, s1.b.x, s2.a.x, s2.b.x) &&
            collides(s1.a.y, s1.b.y, s2.a.y, s2.b.y) &&
            collides(s1.a.z, s1.b.z, s2.a.z, s2.b.z);
}

Shape::Shape(const PT a_, const PT b_): a(min(a_,b_)), b(max(a_,b_))  {}

Shape Shape::expand(int spacing) const {
    return Shape(PT(a.x - spacing, a.y - spacing, a.z), PT(b.x + spacing, b.y + spacing, b.z));
}

}