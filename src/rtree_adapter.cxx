#include <rtree_adapter.hpp>

#include <set>

namespace geometry{


void RTree::populate(const std::vector<Shape> &param) {
    for (const Shape &s : param) {
        add(s);
    }
}

void RTree::add(const Shape &shape) {
    tree.Insert(shape.p1.coords.begin(), shape.p2.coords.begin(), &shape);
}

bool RTree::hits(const PT l, const PT r) const {
    bool found = false;
    const Shape center{l, r};
    this->visit(center, [&found] (const Shape * x) ->bool {
        found = true;
        return false;
    });
    return found;    
}

std::vector<Shape> RTree::neighboors_diamond(const Shape &u, size_t number) const {

    std::vector<Shape> res;

    int a = 0, b = 1;
    const int MAXB = 1e8;
    while (res.size() < number && b < MAXB) {
        visit_diamond_2(u, a, b, [&res](const Shape * v) {
            // if(collides(u, v)) {
            res.push_back(*v);
            // }
            return true;
        });
        a = b;
        b = b*2;
    }
    return res;
}





}