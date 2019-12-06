#include <rtree_adapter.hpp>

namespace geometry{


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

    // while the item to be popped is not a leaf
    while(!queue.empty() && queue.begin()->second->m_child != NULL) {
        auto [k, branch] = *queue.begin();
        queue.erase(queue.begin());
        for(int i = 0; i < branch->m_child->m_count; ++i) {
            push(branch->m_child->m_branch[i]);
        }
    }

    auto [k, branch] = *queue.begin();
    queue.erase(queue.begin());
    return branch->m_data;

    // auto [k, branch] = *queue.begin();
    // queue.erase(queue.begin());

    // if(branch->m_child == NULL){
    //     return branch->m_data;    
    // }

    // for(int i = 0; i < branch->m_child->m_count; ++i) {
    //     push(branch->m_child->m_branch[i]);
    // }

    // return pop();
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