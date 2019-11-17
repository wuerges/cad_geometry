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



}