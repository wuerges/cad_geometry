#pragma once

#include <geometry.hpp>
#include <RTree.h>

#include <vector>


namespace geometry {

using std::vector;

void to_intv(const PT & p, int* v);

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
  

    // template<typename Func>
    // int visit(const Shape & center, 
    //     const Func& f) const ;

    // template<typename Func>
    // int visit_diamond(const Shape & center, unsigned radius, 
    //     const Func& f) const;
    
    // template<typename Func>
    // int visit_diamond_2(const Shape & center, unsigned radius1, unsigned radius2, 
    //     const Func& f) const;
    

    MyTree::Rect to_rect(const Shape & s) const {
        MyTree::Rect r;
        to_intv(s.a, r.m_min);    
        to_intv(s.b, r.m_max);
        return r;
    }


    template<typename Func>
    int visit_diamond(const Shape & center, unsigned radius, 
        const Func& f
        ) const {
        auto p = to_rect(center);    
        return tree.SearchDiamond(p.m_min, p.m_max, radius, [&f,this](int i)->bool { return f(shapes[i]); });
    }

    template<typename Func>
    int visit_diamond_2(const Shape & center, unsigned radius1, unsigned radius2, 
        const Func& f
        ) const {
        auto p = to_rect(center);    
        return tree.SearchDiamond(p.m_min, p.m_max, radius1,  radius2, [&f,this](int i)->bool { return f(shapes[i]); });
    }

    template<typename Func>
    int visit(const Shape & center, 
        const Func& f) const {
        auto p = to_rect(center);    
        return tree.Search(p.m_min, p.m_max, [&f,this](int i) { return f(shapes[i]); });
    }
};






}

