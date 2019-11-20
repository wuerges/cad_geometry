#pragma once
#include <iostream>
#include <array>
#include <optional>

namespace geometry {

template <int N>
struct Point {
    Point() {}
    Point(const int cs[N]) {
      for(int i = 0; i < N; ++i) {
        coords[i] = cs[i];
      }
    }
    Point(const std::initializer_list<int> & cs) {
      int i = 0;
      for(int x : cs) {
        coords[i++] = x;
      }
    }

    std::array<int, N> coords;
    int operator[](const int i) const {
        return coords[i];
    }

};


template<int N>
Point<N> min(const Point<N> & a, const Point<N> &b) {
    Point<N> c;
    for(int i = 0; i < N; ++i) {
        c.coords[i] = std::min(a[i], b[i]);
    }
    return c;
}

template<int N>
Point<N> max(const Point<N> & a, const Point<N> &b) {
    Point<N> c;
    for(int i = 0; i < N; ++i) {
        c.coords[i] = std::max(a[i], b[i]);
    }
    return c;
}

template<int N>
bool operator<(const Point<N> & v1, const Point<N> & v2) {
    return v1.coords < v2.coords;        
}


template<int N>
std::ostream & operator<<(std::ostream & out, const Point<N> & p) {
    out << "(";
    for(int i = 0; i < N; ++i) {
        out << (i==0 ? "": " ") << p[i];
    }
    out << ")";
    return out;
}

template <int N>
struct Rectangle {
    Point<N> p1, p2;
    
    Rectangle(const Point<N> & p1_, const Point<N> & p2_)
    : p1(min(p1_, p2_))
    , p2(max(p1_, p2_)) {}
    Rectangle() {}

    Rectangle<N> expand(int spacing) const;

    double area() const {
      double r = 1;
      for(int i = 0; i < N; ++i) {
        r *= abs(p1[i] - p2[i]);
      }
      return r;
    }

};



template<int N>
bool operator==(const Point<N> & p1, const Point<N> & p2) {
    return p1.coords == p2.coords;
}

template<int N>
bool operator==(const Rectangle<N> & r1, const Rectangle<N> & r2) {
    return r1.p1 == r2.p1 && r1.p2 == r2.p2;
}

template<int N>
bool operator!=(const Rectangle<N> & r1, const Rectangle<N> & r2) {
    return r1.p1 != r2.p1 || r1.p2 != r2.p2;
}

template<int N>
bool operator!=(const Point<N> & p1, const Point<N> & p2) {
    return p1.coords != p2.coords;
}

template<int N>
std::optional<Rectangle<N>> 
intersection(const Rectangle<N> & a, const Rectangle<N> & b) {
    Rectangle<N> result{max(a.p1, b.p1), min(a.p2, b.p2)};

    for(int i = 0; i < N; ++i) {
        if(result.p1[i] > result.p2[i]) {
            return std::nullopt;
        }
    }

    return result;
}


template<int N>
bool operator<(const Rectangle<N> & v1, const Rectangle<N> & v2) {
    if(v1.p1 != v2.p1) {
        return v1.p1 < v2.p1;
    }
    return v1.p2 < v2.p2;
}

template<int N>
std::ostream & operator<<(std::ostream & out, const Rectangle<N> & r) {
    out << "Rectangle(" << r.p1 <<"," << r.p2 << ")";
    return out;
}

template<int N> 
Point<N> crossing(const Rectangle<N> & r1, const Rectangle<N> & r2) {
    Point<N> ret = r2.p1;

    // If r1 is horizontal;
    if(r1.p1[0] == r1.p2[0]) {
        ret.coords[0] = r1.p1[0];
    }
    // If r1 is vertical;
    else {
        ret.coords[1] = r1.p1[1];
    }
    return ret;
}

template<int N>
int distance(const Point<N> & p1, const Point<N> & p2) {
    int result = 0;
    for(int i = 0; i < N; ++i) {
        int p1i = p1[i];
        int p2i = p2[i];
        result += std::abs(p1i - p2i);
    }
    return result;
}

template<int N>
int manhatan(const Point<N> & p1, const Point<N> & p2) {
    return distance<N>(p1, p2);
}

int disti(int a, int b, int aw, int bw);

template<int N>
int distance(const Rectangle<N> & r1, const Rectangle<N> & r2) {

    int result = 0;
    for(int i = 0; i < N; ++i) {
        int ax1 = r1.p1[i];
        int ax2 = r1.p2[i];
        int adx = std::abs(ax1 - ax2);
        
        int bx1 = r2.p1[i];
        int bx2 = r2.p2[i];
        int bdx = std::abs(bx1 - bx2);
        
        result += disti(ax1, bx1, adx, bdx);
    }
    // std::cout << "r1 =" << r1 << std::endl;
    // std::cout << "r2 =" << r2 << std::endl;
    // std::cout << "distance = " << result << std::endl;
    return result;
}

template<int N>
int distance(const Point<N> & r1, const Rectangle<N> & r2) {
  return distance(Rectangle<N>(r1, r1), r2);
}

const bool collides(const int ax1, const int ax2, const int bx1, const int bx2);
// const bool collides(const int ax1, const int ax2, const int bx1, const int bx2) {
//     return (ax1 <= bx1 && bx1 <= ax2) 
//         || (ax1 <= bx2 && bx2 <= ax2) 
//         || (bx1 <= ax1 && ax1 <= bx2);
// }

template<int N>
const bool collides(const Rectangle<N> & s1, const Rectangle<N> & s2) {
  for(int i = 0; i < N; ++i) {
    if(!collides(s1.p1[i],s1.p2[i], s2.p1[i], s2.p2[i])) {
      return false;
    }
  }
  return true;
    // return collides(s1.a[0], s1.b[0], s2.a[0], s2.b[0]) &&
    //         collides(s1.a[1], s1.b[1], s2.a[1], s2.b[1]) &&
    //         collides(s1.a[2], s1.b[2], s2.a[2], s2.b[2]);
}

template<int N>
const bool collides(const Point<N> & p, const Rectangle<N> & s2) {
  for(int i = 0; i < N; ++i) {
    if(!collides(p[i],p[i], s2.p1[i], s2.p2[i])) {
      return false;
    }
  }
  return true;
    // return collides(s1.a[0], s1.b[0], s2.a[0], s2.b[0]) &&
    //         collides(s1.a[1], s1.b[1], s2.a[1], s2.b[1]) &&
    //         collides(s1.a[2], s1.b[2], s2.a[2], s2.b[2]);
}


using R2 = Rectangle<2>;
using R3 = Rectangle<3>;
using P2 = Point<2>;
using P3 = Point<3>;


template<int N>
Rectangle<N> square_around(const Point<N> & p, const int distance = 100) {
    Rectangle<N> r{p, p};
    for(int i = 0; i < N; ++i) {
        r.p1.coords[i] -= distance;
        r.p2.coords[i] += distance;
    }
    return r;
}

    using std::ostream;
    
    // struct PT {
    //   PT(const int v[3]) {
    //     coords[0] = v[0];
    //     coords[1] = v[1];
    //     coords[2] = v[2];
    //   }
    //   PT(int _x, int _y, int _z) {
    //     coords[0] = _x;
    //     coords[1] = _y;
    //     coords[2] = _z;

    //   }
    //   PT() {}

    //   std::array<int, 3> coords;
    //   int operator[](int i) const;

    //   friend ostream & operator<<(ostream &out, const PT & p);

    //   friend const bool operator<(const PT & a, const PT & b);
    //   friend const bool operator==(const PT & a, const PT & b);
    //   friend const bool operator!=(const PT & a, const PT & b);
    //   friend const PT min(const PT & a, const PT & b);
    //   friend const PT max(const PT & a, const PT & b);

    //   friend const int manhatan(const PT & a, const PT & b);      
    // };

    using PT = Point<3>;
    /**
     * Two points that represent the oposite points of a cuboid
     * The first point is the low value for the x, y and z coordinates.
     * The second point is the high value for the x, y, and z coordinates.
     */



    // struct Shape {
    //   Shape(const PT _a, const PT _b);      
    //   Shape() {}
    //   PT a, b;
    //   friend const bool collides(const Shape & a, const Shape & b);
    //   friend const bool collides(const PT & p, const Shape & b);
    //   friend std::ostream & operator<<(std::ostream & out, const Shape & s);

    //   friend const bool operator<(const Shape & a, const Shape & b);
    //   friend const bool operator==(const Shape & a, const Shape & b);
    //   friend const bool operator!=(const Shape & a, const Shape & b);

    //   friend const int distance(const Shape & a, const Shape & b);
    //   friend const int distance(const PT & pt, const Shape & s2);

    //   Shape expand(int spacing) const ;

    //   friend const Shape minimumBound(const Shape & s1, const Shape & s2);

    //   double area() const;
    // };    

    using Shape = Rectangle<3>;


template<int N>
const bool diamond_collides(const Rectangle<N> & center, int radius32, const Point<N> low, const Point<N> high) {
  return distance(center, Rectangle<N>{low, high}) <= radius32;
}

template<int N>
const bool diamond_contains(const Rectangle<N> & center, int radius32, const Point<N> low, const Point<N> high) {
  Point<N> a{low[0], high[1], low[2]};
  Point<N> b{high[0], low[1], low[2]};
  return distance(low, center) <= radius32 
    && distance(high, center) <= radius32
    && distance(a, center) <= radius32
    && distance(b, center) <= radius32;
}

template<int N>
const Rectangle<N> minimumBound(const Rectangle<N> & s1, const Rectangle<N> & s2) {
    return Rectangle<N>(min(s1.p1, s2.p1), max(s1.p2, s2.p2));
}


// const bool sphere_collides(const PT center, int radius32, const int64_t y, const int z, const int64_t x1, const int64_t x2);
// const bool sphere_collides(const PT center, int radius32, const PT low,
//                            const PT high);
// const bool sphere_contains(const PT center, int radius, const PT a, const PT b);

// const bool diamond_collides(const Shape & center, int radius32, const PT low,
//                            const PT high);
// const bool diamond_contains(const Shape & center, int radius32, const PT low,
//                            const PT high);


}

namespace std {
  using geometry::Rectangle;

  template <int N>
  struct hash<Rectangle<N>>
  {
    std::size_t operator()(const Rectangle<N>& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      size_t result = 0;
      for(int i = 0; i < N; ++i) {
        result = result ^ hash<int>()(k.p1[i]) ^ hash<int>()(k.p2[i]);
      }
      return result;

      // return hash<int>()(k.a[0]) ^ hash<int>()(k.a[1]) ^ hash<int>()(k.a[2]) 
      //      ^ hash<int>()(k.b[0]) ^ hash<int>()(k.b[1]) ^ hash<int>()(k.b[2]) ;
    }
  };

}

// namespace std {
//   using geometry::Shape;

//   template <>
//   struct hash<Shape>
//   {
//     std::size_t operator()(const Shape& k) const
//     {
//       using std::size_t;
//       using std::hash;
//       using std::string;

//       // Compute individual hash values for first,
//       // second and third and combine them using XOR
//       // and bit shifting:

//       return hash<int>()(k.a[0]) ^ hash<int>()(k.a[1]) ^ hash<int>()(k.a[2]) 
//            ^ hash<int>()(k.b[0]) ^ hash<int>()(k.b[1]) ^ hash<int>()(k.b[2]) ;
//     }
//   };

// }

