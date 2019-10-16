#pragma once

#include <geometry.hpp>
#include <RTree.h>

#include <vector>


namespace geometry {

using std::vector;



class RTree {
    using MyTree = rtree::RTree<int, int, 3, double>;

    MyTree tree;
    vector<Shape> shapes;

public:
    void populate(const std::vector<Shape> &param);

    void add(const Shape &shape);

    template<typename Func >
    int visit_diamond(const Shape & center, int radius, Func f);
    
};

void to_intv(const PT & p, int* v) {
    v[0] = p.x;
    v[1] = p.y;
    v[2] = p.z;
}

}