#include <geometry.hpp>
#include <iostream>
#include <array>
#include <cassert>
#include <cmath>

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

// const bool operator!=(const Shape & a, const Shape & b) {
//     return ! (a == b);
// }

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
    if(a.x != b.x) return a.x < b.x;
    if(a.y != b.y) return a.y < b.y;
    return a.z < b.z;
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



const bool sphere_collides(const PT center, int radius32, const int64_t y, const int z, const int64_t x1, const int64_t x2) {
  // (x - h)^2 + (y - k)^2 + (z - l)^2 == r^2
  
  int64_t h = center.x;
  int64_t k = center.y;
  int64_t l = center.z;
  int64_t r = radius32;

  int64_t a = 1;  
  int64_t b = - 2 * h;

  int64_t c = h*h + (y-k)*(y-k) + (z-l)*(z-l) - r*r;

  // has solution if b^2 - 4ac >= 0

  //std::printf("y=%d z=%d x1=%d x2=%d\n", y, z, x1, x2);
  if (b*b - 4*a*c < 0) {
    //std::cout << "infeasible\n";
    return false;
  }

  double delta = b*b - 4*a*c;
  double root1 = (-b+sqrt(delta))/(2*a);
  double root2 = (-b-sqrt(delta))/(2*a);
  // std::cout << "roots: " << root1 << ", " << root2 << '\n';
  return (x1 >= root1 && root1 >= x2) || (x1 >= root2 && root2 >= x2);
}



const bool sphere_collides(const PT center, int radius32, const PT low,
                           const PT high) {
  
  const PT transposed{center.y, center.x, center.z};

  return sphere_collides(center, radius32, low.y, low.z, high.x, low.x)
    ||  sphere_collides(center, radius32, high.y, low.z, high.x, low.x)
    || sphere_collides(transposed, radius32, low.x, low.z, high.y, low.y)
    || sphere_collides(transposed, radius32, high.x, low.z, high.y, low.y)
    || sphere_contains(center, radius32, low, high);

}

const bool diamond_collides(const Shape & center, int radius32, const PT low, const PT high) {
  return distance(center, Shape{low, high}) <= radius32;
}
const bool diamond_contains(const Shape & center, int radius32, const PT low, const PT high) {
  PT a{low.x, high.y, low.z};
  PT b{high.x, low.y, low.z};
  return distance(low, center) <= radius32 
    && distance(high, center) <= radius32
    && distance(a, center) <= radius32
    && distance(b, center) <= radius32;
}


const bool sphere_contains(const PT center, int radius32, const PT low,
                           const PT high) {
  int64_t radius = radius32;
  int64_t s_radius = radius * radius;
  for (int x : {high.x, low.x}) {
    for (int y : {high.y, low.y}) {
      for (int z : {high.z, low.z}) {
        int64_t dx = (x - center.x);
        int64_t dy = (y - center.y);
        int64_t dz = (z - center.z);
        int64_t p_radius = dx * dx + dy * dy + dz * dz;
        if (p_radius > s_radius) {
          return false;
        }
      }
    }
  }
  return true;
}


}


