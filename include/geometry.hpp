#pragma once
#include <iostream>
#include <array>

namespace geometry {

    using std::ostream;
    
    struct PT {
      PT(const int v[3]) {
        coords[0] = v[0];
        coords[1] = v[1];
        coords[2] = v[2];
      }
      PT(int _x, int _y, int _z) {
        coords[0] = _x;
        coords[1] = _y;
        coords[2] = _z;

      }
      PT() {}

      std::array<int, 3> coords;
      int operator[](int i) const;

      friend ostream & operator<<(ostream &out, const PT & p);

      friend const bool operator<(const PT & a, const PT & b);
      friend const bool operator==(const PT & a, const PT & b);
      friend const bool operator!=(const PT & a, const PT & b);
      friend const PT min(const PT & a, const PT & b);
      friend const PT max(const PT & a, const PT & b);

      friend const int manhatan(const PT & a, const PT & b);      
    };
    /**
     * Two points that represent the oposite points of a cuboid
     * The first point is the low value for the x, y and z coordinates.
     * The second point is the high value for the x, y, and z coordinates.
     */

    struct Shape {
      Shape(const PT _a, const PT _b);      
      Shape() {}
      PT a, b;
      friend const bool collides(const Shape & a, const Shape & b);
      friend const bool collides(const PT & p, const Shape & b);
      friend std::ostream & operator<<(std::ostream & out, const Shape & s);

      friend const bool operator<(const Shape & a, const Shape & b);
      friend const bool operator==(const Shape & a, const Shape & b);
      friend const bool operator!=(const Shape & a, const Shape & b);

      friend const int distance(const Shape & a, const Shape & b);
      friend const int distance(const PT & pt, const Shape & s2);

      Shape expand(int spacing) const ;

      friend const Shape minimumBound(const Shape & s1, const Shape & s2);

      double area() const;

    };    


const bool sphere_collides(const PT center, int radius32, const int64_t y, const int z, const int64_t x1, const int64_t x2);
const bool sphere_collides(const PT center, int radius32, const PT low,
                           const PT high);
const bool sphere_contains(const PT center, int radius, const PT a, const PT b);

const bool diamond_collides(const Shape & center, int radius32, const PT low,
                           const PT high);
const bool diamond_contains(const Shape & center, int radius32, const PT low,
                           const PT high);

}

namespace std {
  using geometry::Shape;

  template <>
  struct hash<Shape>
  {
    std::size_t operator()(const Shape& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return hash<int>()(k.a[0]) ^ hash<int>()(k.a[1]) ^ hash<int>()(k.a[2]) 
           ^ hash<int>()(k.b[0]) ^ hash<int>()(k.b[1]) ^ hash<int>()(k.b[2]) ;
    }
  };

}

