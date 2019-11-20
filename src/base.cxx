#include <geometry.hpp>
// #include <iostream>
// #include <array>
// #include <cassert>
// #include <cmath>

namespace geometry {

template<>
Rectangle<3> Rectangle<3>::expand(int spacing) const {
    return Rectangle<3>(
      Point<3>({p1[0] - spacing, p1[1] - spacing, p1[2]}), 
      Point<3>({p2[0] + spacing, p2[1] + spacing, p2[2]}));
}

template<>
Rectangle<2> Rectangle<2>::expand(int spacing) const {
    return Rectangle<2>(
      Point<2>({p1[0] - spacing, p1[1] - spacing}), 
      Point<2>({p2[0] + spacing, p2[1] + spacing}));
}

const bool collides(const int ax1, const int ax2, const int bx1, const int bx2) {
    return (ax1 <= bx1 && bx1 <= ax2) 
        || (ax1 <= bx2 && bx2 <= ax2) 
        || (bx1 <= ax1 && ax1 <= bx2);
}

// std::ostream & operator<<(std::ostream & out, const Shape & s) {
//     out << "Shape{"<<s.p1<", "<<s.p2<<"}";
//     // out << "Polygon(";
//     // out << "(" << s.a[0] << "," << s.a[1] << "),";
//     // out << "(" << s.b[0] << "," << s.a[1] << "),";
//     // out << "(" << s.b[0] << "," << s.b[1] << "),";
//     // out << "(" << s.a[0] << "," << s.b[1] << ")";
//     // out << ")";
//     return out;
// }
// std::ostream & operator<<(std::ostream & out, const PT & s) {
//     out << "PT{"<<s[0]<<", "<<s[1]<<", "<<s[2]<<"}";
//     return out;
// }


// int PT::operator[](int i) const {
//     return coords[i]; 
// }

// const int manhatan(const PT & a, const PT & b) {
//     return abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2]);
// }

// const bool operator<(const Shape & a, const Shape & b) {
//     if(a.a != b.a) {
//         return a.a < b.a;        
//     }
//     return a.b < b.b;
// }

// const bool operator==(const Shape & a, const Shape & b) {
//     return a.a == b.a && a.b == b.b;
// }
// const bool operator==(const PT & a, const PT & b) {
//     return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
// }
// const bool operator!=(const PT & a, const PT & b) {
//     return a[0] != b[0] || a[1] != b[1] || a[2] != b[2];
// }

// const bool operator!=(const Shape & a, const Shape & b) {
//     return a.a != b.a || a.b != b.b;
// }

int disti(int a, int b, int aw, int bw) {
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


// const int distance(const Shape & s1, const Shape & s2) {
//     return dist(s1.a[0], s2.a[0], abs(s1.b[0] - s1.a[0]),  abs(s2.b[0] - s2.a[0]))
//             + dist(s1.a[1], s2.a[1], abs(s1.b[1] - s1.a[1]),  abs(s2.b[1] - s2.a[1]))
//             + dist(s1.a[2], s2.a[2], abs(s1.b[2] - s1.a[2]),  abs(s2.b[2] - s2.a[2]));
//             // + abs(s1.a[2] - s2.b[2]);
// }

// const int distance(const PT & pt, const Shape & s2) {
//     return distance(Shape(pt, pt), s2);
// }

// const bool operator<(const PT & a, const PT & b) {
//     if(a[0] != b[0]) return a[0] < b[0];
//     if(a[1] != b[1]) return a[1] < b[1];
//     return a[2] < b[2];
// }

// const PT min(const PT & a, const PT & b) {
//     using std::min;
//     return PT(
//         min(a[0], b[0]),
//         min(a[1], b[1]),
//         min(a[2], b[2]));
// }

// const PT max(const PT & a, const PT & b) {
//     using std::max;
//     return PT(
//         max(a[0], b[0]),
//         max(a[1], b[1]),
//         max(a[2], b[2]));
// }

// const bool collides(const PT & p, const Shape & s2) {
//     return collides(Shape{p,p}, s2);
// }

// const bool collides(const int ax1, const int ax2, const int bx1, const int bx2) {
//     return (ax1 <= bx1 && bx1 <= ax2) 
//         || (ax1 <= bx2 && bx2 <= ax2) 
//         || (bx1 <= ax1 && ax1 <= bx2);
// }

// const bool collides(const Shape & s1, const Shape & s2) {
//     return collides(s1.a[0], s1.b[0], s2.a[0], s2.b[0]) &&
//             collides(s1.a[1], s1.b[1], s2.a[1], s2.b[1]) &&
//             collides(s1.a[2], s1.b[2], s2.a[2], s2.b[2]);
// }

// Shape::Shape(const PT a_, const PT b_): a(min(a_,b_)), b(max(a_,b_))  {}

// Shape Shape::expand(int spacing) const {

//     return Shape(PT({a[0] - spacing, a[1] - spacing, a[2]}), PT({b[0] + spacing, b[1] + spacing, b[2]}));
// }



// const bool sphere_collides(const PT center, int radius32, const int64_t y, const int z, const int64_t x1, const int64_t x2) {
//   // (x - h)^2 + (y - k)^2 + (z - l)^2 == r^2
  
//   int64_t h = center[0];
//   int64_t k = center[1];
//   int64_t l = center[2];
//   int64_t r = radius32;

//   int64_t a = 1;  
//   int64_t b = - 2 * h;

//   int64_t c = h*h + (y-k)*(y-k) + (z-l)*(z-l) - r*r;

//   // has solution if b^2 - 4ac >= 0

//   //std::printf("y=%d z=%d x1=%d x2=%d\n", y, z, x1, x2);
//   if (b*b - 4*a*c < 0) {
//     //std::cout << "infeasible\n";
//     return false;
//   }

//   double delta = b*b - 4*a*c;
//   double root1 = (-b+sqrt(delta))/(2*a);
//   double root2 = (-b-sqrt(delta))/(2*a);
//   // std::cout << "roots: " << root1 << ", " << root2 << '\n';
//   return (x1 >= root1 && root1 >= x2) || (x1 >= root2 && root2 >= x2);
// }



// const bool sphere_collides(const PT center, int radius32, const PT low,
//                            const PT high) {
  
//   const PT transposed{center[1], center[0], center[2]};

//   return sphere_collides(center, radius32, low[1], low[2], high[0], low[0])
//     ||  sphere_collides(center, radius32, high[1], low[2], high[0], low[0])
//     || sphere_collides(transposed, radius32, low[0], low[2], high[1], low[1])
//     || sphere_collides(transposed, radius32, high[0], low[2], high[1], low[1])
//     || sphere_contains(center, radius32, low, high);

// }

// const bool diamond_collides(const Shape & center, int radius32, const PT low, const PT high) {
//   return distance(center, Shape{low, high}) <= radius32;
// }
// const bool diamond_contains(const Shape & center, int radius32, const PT low, const PT high) {
//   PT a{low[0], high[1], low[2]};
//   PT b{high[0], low[1], low[2]};
//   return distance(low, center) <= radius32 
//     && distance(high, center) <= radius32
//     && distance(a, center) <= radius32
//     && distance(b, center) <= radius32;
// }


// const bool sphere_contains(const PT center, int radius32, const PT low,
//                            const PT high) {
//   int64_t radius = radius32;
//   int64_t s_radius = radius * radius;
//   for (int x : {high[0], low[0]}) {
//     for (int y : {high[1], low[1]}) {
//       for (int z : {high[2], low[2]}) {
//         int64_t dx = (x - center[0]);
//         int64_t dy = (y - center[1]);
//         int64_t dz = (z - center[2]);
//         int64_t p_radius = dx * dx + dy * dy + dz * dz;
//         if (p_radius > s_radius) {
//           return false;
//         }
//       }
//     }
//   }
//   return true;
// }

// template<int N>
// const Shape minimumBound(const Shape & s1, const Shape & s2) {
//     return Shape(min(s1.a, s2.a), max(s1.b, s2.b));
// }

// double Shape::area() const {

//     double dx = abs(a[0] - b[0]);
//     double dy = abs(a[1] - b[1]);
//     double dz = abs(a[2] - b[2]);
//     return dx * dy * dz;
// }

}