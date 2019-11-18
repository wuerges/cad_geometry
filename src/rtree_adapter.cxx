#include <rtree_adapter.hpp>

#include <set>

namespace geometry{


void RTree::populate(const std::vector<Shape> &param) {
    for (const Shape &s : param) {
        add(s);
    }
}

void RTree::add(const Shape &shape) {
    tree.Insert(shape.a.coords.begin(), shape.b.coords.begin(), &shape);
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

int RTreeQueue::peek() const {
    if(queue.empty()) {
        return 1e9;
    }
    return queue.begin()->first;

}
bool RTreeQueue::empty() const {
    return queue.empty();
}
const Shape * RTreeQueue::pop() {
    auto [k, branch] = *queue.begin();
    queue.erase(queue.begin());

    if(branch->m_child == NULL){
        return branch->m_data;    
    }

    for(int i = 0; i < branch->m_child->m_count; ++i) {
        push(branch->m_child->m_branch[i]);
    }

    return pop();
}
void RTreeQueue::push(const RTree::MyTree::Branch & branch)  {
    Shape aux(PT(branch.m_rect.m_min), PT(branch.m_rect.m_max));
    queue.emplace(distance(aux, center), &branch);
}

RTreeQueue::RTreeQueue(const Shape & c, const RTree & t)
:center(c) {
    const RTree::MyTree::Node * root = t.tree.m_root;
    for(int i = 0; i < root->m_count; ++i) {
        push(root->m_branch[i]);
    }
}

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