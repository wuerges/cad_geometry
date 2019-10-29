#pragma once

#include <geometry.hpp>
#include <RTree.h>

#include <vector>
#include <set>

namespace geometry {

using std::vector;

template <int NDIM>
class RTree {

    using Shapei = Shape<NDIM>;
    using PTi = PT<NDIM>;

    vector<Shapei> shapes;

public:
    using MyTree = rtree::RTree<int, int, NDIM, double>;
    MyTree tree;
    
    void populate(const std::vector<Shapei> &param) {
        for (const Shapei &s : param) {
            add(s);
        }
    }

    void add(const Shapei &shape) {
        int id = shapes.size();
        shapes.push_back(shape);
        
        int v1[NDIM], v2[NDIM];
        to_intv(shape.a, v1);
        to_intv(shape.b, v2);
        
        tree.Insert(v1, v2, id);
    }

    // bool hits(const PTi l, const PTi r) const;
    // std::vector<Shapei> collect(const PTi l, const PTi r) const;
    // std::vector<Shapei> neighboors_diamond(const Shapei &u, size_t number) const;
    // std::vector<Shapei> collect_diamond(const Shapei & center, unsigned radius) const;
    // std::vector<Shapei> collect_diamond_2(const Shapei & center, unsigned radius1, unsigned radius2) const;
  

    int visit(const Shapei & center, 
        const std::function <bool (const Shapei &)>& f) const {
        auto p = to_rect(center);    
        return tree.Search(p.m_min, p.m_max, [&f,this](int i) { return f(shapes[i]); });
    }

    int visit_diamond(const Shapei & center, unsigned radius, 
        const std::function <bool (const Shapei &)>& f) const {
        auto p = to_rect(center);    
        return tree.SearchDiamond(p.m_min, p.m_max, radius, [&f,this](int i)->bool { return f(shapes[i]); });
    }

    int visit_diamond_2(const Shapei & center, unsigned radius1, unsigned radius2, 
        const std::function <bool (const Shapei &)>& f) const {
        auto p = to_rect(center);
        return tree.SearchDiamond(p.m_min, p.m_max, radius1,  radius2, [&f,this](int i)->bool { return f(shapes[i]); });
    }
  

    bool hits(const PTi l, const PTi r) const {
        bool found = false;
        const Shapei center{l, r};
        this->visit(center, [&found] (const Shapei & x) ->bool {
            found = true;
            return false;
        });
        return found;    
    }
    std::vector<Shapei> collect(const PTi l, const PTi r) const {
        std::vector<Shapei> res;
        
        const Shapei center{l,r};
        visit(center, [&res](const Shapei & x) {
            res.push_back(x);
            return true;
        });
        return res;
    }

    std::vector<Shapei> neighboors_diamond(const Shapei &u, size_t number) const {

        std::vector<Shapei> res;

        int a = 0, b = 1;
        const int MAXB = 1e8;
        while (res.size() < number && b < MAXB) {
            visit_diamond_2(u, a, b, [&res](const Shapei & v) {
                // if(collides(u, v)) {
                res.push_back(v);
                // }
                return true;
            });
            a = b;
            b = b*2;
        }
        return res;
    }


    std::vector<Shapei> collect_diamond(const Shapei & u, unsigned radius) const {
        std::vector<Shapei> res;
        visit_diamond(u, radius, [&res](const Shapei & v) {
            res.push_back(v);
            return true;
        });
        return res; 

    }
    std::vector<Shapei> collect_diamond_2(const Shapei & u, unsigned radius1, unsigned radius2) const {
    std::vector<Shapei> res;
        visit_diamond_2(u, radius1, radius2, [&res](const Shapei & v) {
            res.push_back(v);
            return true;
        });
        return res; 
    }
};

template <int NDIM>
void to_intv(const PT<NDIM> & p, int* v) {
    for(int i = 0; i < NDIM; ++i) {
        v[i] = p[i];
    }
}

template <int NDIM>
typename RTree<NDIM>::MyTree::Rect to_rect(const Shape<NDIM> & s) {
    typename RTree<NDIM>::MyTree::Rect r;
    to_intv(s.a, r.m_min);
    to_intv(s.b, r.m_max);
    return r;
}


}

