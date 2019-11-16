#include <rtree_adapter.hpp>

#include <set>

namespace geometry{


void RTree::populate(const std::vector<Shape> &param) {
    for (const Shape &s : param) {
        add(s);
    }
}

void RTree::add(const Shape &shape) {
    int id = shapes.size();
    shapes.push_back(shape);
    
    tree.Insert(shape.a.coords.begin(), shape.b.coords.begin(), id);
}

bool RTree::hits(const PT l, const PT r) const {
    bool found = false;
    const Shape center{l, r};
    this->visit(center, [&found] (const Shape & x) ->bool {
        found = true;
        return false;
    });
    return found;    
}
std::vector<Shape> RTree::collect(const PT l, const PT r) const {
    std::vector<Shape> res;
    std::set<Shape> temp;
    
    const Shape center{l,r};
    visit(center, [&temp](const Shape & x) {
        temp.insert(x);
        return true;
    });
    for(auto & x : temp) { res.push_back(x); }
    return res;
}

std::vector<Shape> RTree::neighboors_diamond(const Shape &u, size_t number) const {

    std::vector<Shape> res;

    int a = 0, b = 1;
    const int MAXB = 1e8;
    while (res.size() < number && b < MAXB) {
        visit_diamond_2(u, a, b, [&res](const Shape & v) {
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


std::vector<Shape> RTree::collect_diamond(const Shape & u, unsigned radius) const {
    std::vector<Shape> res;
    visit_diamond(u, radius, [&res](const Shape & v) {
        res.push_back(v);
        return true;
    });
    return res; 

}
std::vector<Shape> RTree::collect_diamond_2(const Shape & u, unsigned radius1, unsigned radius2) const {
   std::vector<Shape> res;
    visit_diamond_2(u, radius1, radius2, [&res](const Shape & v) {
        res.push_back(v);
        return true;
    });
    return res; 
}

}