#pragma once

#include <geometry.hpp>
#include <RTree.h>

#include <vector>
#include <map>
#include <list>

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
        return tree.SearchDiamond(center.p1.coords.begin(), center.p2.coords.begin(), radius, f);
    }

    template<typename Func>
    inline int visit_diamond_2(const Shape & center, unsigned radius1, unsigned radius2, 
        const Func& f
        ) const {
        return tree.SearchDiamond(center.p1.coords.begin(), center.p2.coords.begin(), radius1,  radius2, f);
    }

    template<typename Func>
    inline int visit(const Shape & center, 
        const Func& f) const {
        return tree.Search(center.p1.coords.begin(), center.p2.coords.begin(), f);
    }
};

struct RTreeQueue {
    RTreeQueue(const Shape & c, const RTree & t);
    int peek() const ;
    const Shape * pop();
    bool empty() const;
    const Shape center;
private:    
    void push(const RTree::MyTree::Branch&);
    std::multimap<const int, const RTree::MyTree::Branch*> queue;
};

struct RTreeQueueFilter {

    /**
     * Creates a vertex queue.
     * 
     * @param u is the vertexs that defines adjacency.
     * @param c is the target vertex.
     * @param t is the tree.
     */
    RTreeQueueFilter(const Shape & u, const Shape & c, const RTree & t);
    int peek() const ;
    const Shape * pop();
    bool empty() const;
    const Shape center;
    const Shape u;
private:    
    void push(const RTree::MyTree::Branch&);
    std::multimap<const int, const RTree::MyTree::Branch*> queue;
};

struct RTreeQueue2 {
    RTreeQueue2(const Shape & c, const RTree & t);
    int peek() ;
    const Shape * pop();
    bool empty();
    const Shape center;
private: 
    int radius1;
    int radius2;
    const RTree & tree;
    void load();
    std::list<const Shape*> queue;
};



}

