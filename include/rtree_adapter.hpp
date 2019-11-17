#pragma once

#include <geometry.hpp>
#include <RTree.h>

#include <vector>
#include <multimap>

namespace geometry {

using std::vector;


class RTree {

    vector<Shape> shapes;

public:
    using MyTree = rtree::RTree<int, int, 3, double>;
    MyTree tree;
    void populate(const std::vector<Shape> &param);
    void add(const Shape &shape);

    bool hits(const PT l, const PT r) const;
    std::vector<Shape> collect(const PT l, const PT r) const;
    std::vector<Shape> neighboors_diamond(const Shape &u, size_t number) const;
    std::vector<Shape> collect_diamond(const Shape & center, unsigned radius) const;
    std::vector<Shape> collect_diamond_2(const Shape & center, unsigned radius1, unsigned radius2) const;
  
    template<typename Func>
    inline int visit_diamond(const Shape & center, unsigned radius, 
        const Func& f
        ) const {
        return tree.SearchDiamond(center.a.coords.begin(), center.b.coords.begin(), radius, [&f,this](int i)->bool { return f(shapes[i]); });
    }

    template<typename Func>
    inline int visit_diamond_2(const Shape & center, unsigned radius1, unsigned radius2, 
        const Func& f
        ) const {
        return tree.SearchDiamond(center.a.coords.begin(), center.b.coords.begin(), radius1,  radius2, [&f,this](int i)->bool { return f(shapes[i]); });
    }

    template<typename Func>
    inline int visit(const Shape & center, 
        const Func& f) const {
        return tree.Search(center.a.coords.begin(), center.b.coords.begin(), [&f,this](int i) { return f(shapes[i]); });
    }


};

struct RTreeQueue {
    Shape center;
    
};






}

