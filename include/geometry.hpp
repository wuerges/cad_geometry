#pragma once
#include <iostream>

namespace geometry {

    using std::ostream;
    struct PT {
      PT(int _x, int _y, int _z): x(_x), y(_y), z(_z) {}
      PT() {}

      int x, y, z;
      int operator[](int i) const;

      friend ostream & operator<<(ostream &out, const PT & p);

      friend const bool operator<(const PT & a, const PT & b);
      friend const bool operator<=(const PT & a, const PT & b);
      friend const bool operator>(const PT & a, const PT & b);
      friend const bool operator>=(const PT & a, const PT & b);
      friend const bool operator==(const PT & a, const PT & b);
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
      friend const bool operator!=(const Shape & a, const Shape & b);
      friend const bool operator==(const Shape & a, const Shape & b);

      friend const int distance(const Shape & a, const Shape & b);
      friend const int distance(const PT & pt, const Shape & s2);

      Shape expand(int spacing) const ;

    };    


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

      return hash<int>()(k.a.x) ^ hash<int>()(k.a.y) ^ hash<int>()(k.a.z) 
           ^ hash<int>()(k.b.x) ^ hash<int>()(k.b.y) ^ hash<int>()(k.b.z) ;
    }
  };

}
