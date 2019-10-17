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

    bool hits(const PT l, const PT r) const;
    std::vector<Shape> collect(const PT l, const PT r) const;
    std::vector<Shape> neighboors_diamond(const Shape &u, size_t number) const;
    std::vector<Shape> collect_diamond(const Shape & center, int radius) const;
    std::vector<Shape> collect_diamond_2(const Shape & center, int radius1, int radius2) const;
  

    int visit(const Shape & center, 
        const std::function <bool (const Shape &)>& f) const ;

    int visit_diamond(const Shape & center, int radius, 
        const std::function <bool (const Shape &)>& f) const;
    
    
};

void to_intv(const PT & p, int* v);

}