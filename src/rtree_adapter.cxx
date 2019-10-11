#include <rtree_adapter.hpp>

namespace geometry{


void RTree::populate(const std::vector<Shape> &param) {
    for (const Shape &s : param) {
        add(s);
    }
}

void RTree::add(const Shape &shape) {
    int id = shapes.size();
    shapes.push_back(shape);
    
    int v1[3], v2[3];
    to_intv(shape.a, v1);
    to_intv(shape.b, v2);
    
    tree.Insert(v1, v2, id);
}

template<typename Func >
int RTree::visit_diamond(const Shape & center, int radius, Func f) {
    int v1[3], v2[3];
    to_intv(center.a, v1);
    to_intv(center.b, v2);        
    return tree.SearchDiamond(v1, v2, radius, [&f,this](int i) { return f(shapes[i]); });
}

}