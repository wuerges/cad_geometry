#include <rtree_adapter.hpp>

namespace geometry{

int RTreeQueue2::peek() {
    if(queue.empty()) {
        return 1e9;
    }
    return distance(center, *queue.front());

}
bool RTreeQueue2::empty() {
    load();
    const int INF = 1e9;
    return radius2 >= INF;
}
const Shape * RTreeQueue2::pop() {
    load();
    const Shape * temp = queue.front();
    queue.pop_front();
    return temp;
}
void RTreeQueue2::load()  {
    const int INF = 1e9;
    bool rerun = false;
    while (queue.empty() && radius2 < INF) {
        rerun = true;
        // for(auto v : treap.collect_diamond_2(u, a, b)) {
        //   edges.insert({distance(u, v), u, v});
        // }
        tree.visit_diamond_2(center,radius1,radius2, [&](const Shape * v) {
            queue.push_front(v);
            return true;
        });
        radius1 = radius2;
        radius2 *= 2;
    }
    if(rerun) {
        // sort(queue.begin(), queue.end(), [this](const Shape * s1, const Shape * s2) {
        //     return distance(*s1, center) < distance(*s2, center);
        // });
        queue.sort([this](const Shape * s1, const Shape * s2) {
            return distance(*s1, center) < distance(*s2, center);
        });
    }
}

RTreeQueue2::RTreeQueue2(const Shape & c, const RTree & t)
:center(c), radius1(0), radius2(100), tree(t) {

}


}