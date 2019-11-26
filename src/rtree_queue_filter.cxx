#include <rtree_adapter.hpp>

namespace geometry{


int RTreeQueueFilter::peek() const {
    if(queue.empty()) {
        return 1e9;
    }
    return queue.begin()->first;

}
bool RTreeQueueFilter::empty() const {
    return queue.empty();
}
const Shape * RTreeQueueFilter::pop() {

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

void RTreeQueueFilter::push(const RTree::MyTree::Branch & branch)  {
    Shape aux(PT(branch.m_rect.m_min), PT(branch.m_rect.m_max));
    if(collides(aux, u)) {
        queue.emplace(distance(aux, center), &branch);
    }
}

RTreeQueueFilter::RTreeQueueFilter(const Shape & u, const Shape & c, const RTree & t)
:u(u),center(c) {
    const RTree::MyTree::Node * root = t.tree.m_root;
    for(int i = 0; i < root->m_count; ++i) {
        push(root->m_branch[i]);
    }
}

}