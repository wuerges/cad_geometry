#pragma once
#include <iostream>
#include <array>

using std::ostream, std::array;

namespace geometry {

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


template< int NDIM >
struct PT {
    PT(array<int, NDIM> p): coord(p) {}
    PT() {}

    array<int, NDIM> coord;

    int operator[](int i) const {
        return coord[i];
    };   

    friend ostream & operator<<(ostream & out, const PT & pt) {
        out << "PT{";
        for(int i = 0; i < NDIM; ++i) {
            out << (i==0?"":", ") << pt[i];
        }
        out << "}";
        return out;
    }
    
};

template<int NDIM>
const bool operator<(const PT<NDIM> & a, const PT<NDIM> & b) {
    return a.coord < b.coord;
}

template<int NDIM>
const bool operator==(const PT<NDIM> & a, const PT<NDIM> & b) {
    return a.coord == b.coord;
}

template<int NDIM>
const PT<NDIM> min(const PT<NDIM> & a, const PT<NDIM> & b) {
    using std::min;
    array<int, NDIM> res;
    for(int i = 0; i < NDIM; ++i) {
        res[i] = min(a[i], b[i]);
    }
    return res;
}

template<int NDIM>
const PT<NDIM> max(const PT<NDIM> & a, const PT<NDIM> & b) {
    using std::max;
    array<int, NDIM> res;
    for(int i = 0; i < NDIM; ++i) {
        res[i] = max(a[i], b[i]);
    }
    return res;
}

template<int NDIM>
const int manhatan(const PT<NDIM> & a, const PT<NDIM> & b) {
    using std::abs;
    int res = 0;
    for(int i = 0; i < NDIM; ++i) {
        res += abs(a.coord[i] - b.coord[i]);
    }
    return res;
}

/**
 * Two points that represent the oposite points of a cuboid
 * The first point is the low value for the x, y and z coordinates.
 * The second point is the high value for the x, y, and z coordinates.
 */

template< int NDIM >
class Shape {
    using PTi = PT<NDIM>;
    
    public:

    const PTi a, b;

    Shape(const PTi _a, const PTi _b): a(min(_a,_b)), b(max(_a,_b)) {}
    Shape() {}

    friend ostream & operator<<(ostream &out, const Shape & s) {
        out << "Shape{"<<s.a<<", "<<s.b<<"}";
        // out << "Polygon(";
        // out << "(" << a.x << "," << a.y << "),";
        // out << "(" << b.x << "," << a.y << "),";
        // out << "(" << b.x << "," << b.y << "),";
        // out << "(" << a.x << "," << b.y << ")";
        // out << ")";
        return out;
    }

    Shape expand(int spacing) const {
        return Shape(PTi(a.x - spacing, a.y - spacing, a.z), PTi(b.x + spacing, b.y + spacing, b.z));
    }
};

template<int NDIM>
const bool operator<(const Shape<NDIM> & s1, const Shape<NDIM> & s2) {
    return (s1.a < s2.a) ? true : s1.b < s2.b;
}

template<int NDIM>
const bool operator==(const Shape<NDIM> & s1, const Shape<NDIM> & s2) {
    return s1.a == s2.a && s1.b == s2.b;
}

const bool collides(const int ax1, const int ax2, const int bx1, const int bx2) {
    return (ax1 <= bx1 && bx1 <= ax2) 
        || (ax1 <= bx2 && bx2 <= ax2) 
        || (bx1 <= ax1 && ax1 <= bx2);
}

template<int NDIM>
const bool collides(const Shape<NDIM> & s1, const Shape<NDIM> & s2) {

    for(int i = 0; i < NDIM; ++i) {
        if(!collides(s1.a[i], s1.b[i], s2.a[i], s2.b[i])) {
            return false;
        }
    }

    return true;
}

template<int NDIM>
const bool collides(const PT<NDIM> & p, const Shape<NDIM> & s1) {
    for(int i = 0; i < NDIM; ++i) {
        if(!collides(s1.a[i], s1.b[i], p[i], p[i])) {
            return false;
        }
    }

    return true;
}

template<int NDIM>
const int distance(const Shape<NDIM> & s1, const Shape<NDIM> & s2) {
    int result = 0;

    for(int i = 0; i < NDIM; ++i) {
        result += dist(s1.a[i], s2.a[i], abs(s1.b[i] - s1.a[i]), abs(s2.b[i] - s2.a[i]));
    }

    return result;
}

template<int NDIM>
const int distance(const PT<NDIM> & pt, const Shape<NDIM> & s1) {
    int result = 0;

    for(int i = 0; i < NDIM; ++i) {
        result += dist(s1.a[i], pt[i], abs(s1.b[i] - s1.a[i]), 0);
    }

    return result;
}

template< int NDIM >
const bool diamond_collides(const Shape<NDIM> & center, unsigned radius32, const PT<NDIM> low,
                const PT<NDIM> high) {
    return distance(center, Shape{low, high}) <= radius32;
}

template< int NDIM >
const bool diamond_contains(const Shape<NDIM> & center, unsigned radius32, const PT<NDIM> low,
                const PT<NDIM> high);


/**
 * Assumes that low[2] == low[3]
 */
template<>
const bool diamond_contains(const Shape<3> & center, unsigned radius32, const PT<3> low,
                const PT<3> high)             
                 {

    PT<3> left({low[0], high[1], low[2]});
    PT<3> right({high[0], low[1], low[2]});
    return distance(low, center) <= radius32 
        && distance(high, center) <= radius32
        && distance(left, center) <= radius32
        && distance(right, center) <= radius32;
}


template<>
const bool diamond_contains(const Shape<2> & center, unsigned radius32, const PT<2> low,
                const PT<2> high)             
                 {

    PT<2> left({low[0], high[1]});
    PT<2> right({high[0], low[1]});
    return distance(low, center) <= radius32 
        && distance(high, center) <= radius32
        && distance(left, center) <= radius32
        && distance(right, center) <= radius32;
}

}

namespace std {
    using geometry::Shape;

    template <int NDIM >
    struct hash<Shape<NDIM> >
    {
        std::size_t operator()(const Shape<NDIM>& k) const
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

