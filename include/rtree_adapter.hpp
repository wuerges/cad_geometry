#pragma once

#include <geometry.hpp>
#include <RTree.h>

#include <vector>
#include <map>

namespace geometry {

using std::vector;


class RTree {

    // vector<const Shape> shapes;

public:
    using MyTree = rtree::RTree<const Shape*, int, 3, double>;
    MyTree tree;
    void populate(const std::vector<Shape> &param);
    void add(const Shape &shape);

    bool hits(const PT l, const PT r) const;
    std::vector<Shape> neighboors_diamond(const Shape &u, size_t number) const;
  
    template<typename Func>
    inline int visit_diamond(const Shape & center, unsigned radius, 
        const Func& f
        ) const {
        return tree.SearchDiamond(center.a.coords.begin(), center.b.coords.begin(), radius, f);
    }

    template<typename Func>
    inline int visit_diamond_2(const Shape & center, unsigned radius1, unsigned radius2, 
        const Func& f
        ) const {
        return tree.SearchDiamond(center.a.coords.begin(), center.b.coords.begin(), radius1,  radius2, f);
    }

    template<typename Func>
    inline int visit(const Shape & center, 
        const Func& f) const {
        return tree.Search(center.a.coords.begin(), center.b.coords.begin(), f);
    }


};

struct RTreeQueue {
    RTreeQueue(const Shape & c, const RTree & t);
    int peek() const ;
    const Shape * pop();
private:    
    const Shape center;
    void push(const RTree::MyTree::Branch&);
    std::multimap<const int, const RTree::MyTree::Branch*> queue;
};






}

