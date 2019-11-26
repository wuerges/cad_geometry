#include <geometry.hpp>
#include <RTree.h>

#include <map>
using geometry::Rectangle;
using geometry::Point;
using std::multimap;

template<int N, typename Value> 
struct RTreeQueueTempl {

    using Key = Rectangle<N>;

    int peek() const {
        if(queue.empty()) {
            return 1e9;
        }
        return queue.begin()->first;
    }
    
    const Value * pop() {
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
    
    bool empty() const {
        return queue.empty();
    }
    
    using RTree = rtree::RTree<Value*, int, N, double>;
    
    virtual void push(const typename RTree::Branch & branch) = 0;

    
    const Key center;
    // RTQ<T, N, TYPE> m;


    RTreeQueueTempl(const Key & c, const RTree & t): center(c), tree(t) {}
    // typedef rtree::RTree<T*, int, N, double> RTree;
// private:    
    // void push(const typename RTree::Branch&);
    multimap<const int, const typename RTree::Branch*> queue;
    const RTree & tree;
};


template<int N, typename Value> 
struct RTreeQueueFilter : public RTreeQueueTempl<N, Value> {

    using Key = Rectangle<N>;

    // using Branch = typename RTreeQueueTempl<T, N>::RTree::Branch;
    using RTree = typename RTreeQueueTempl<N, Value>::RTree;

    void push(const typename RTree::Branch & branch) {
        Key aux(Point<N>(branch.m_rect.m_min), Point<N>(branch.m_rect.m_max));
        if(collides(aux, u)) {
            RTreeQueueTempl<N, Value>::queue.emplace(distance(aux, RTreeQueueTempl<N, Value>::center), &branch);
        }
    }

    const Key u;

    RTreeQueueFilter(const Key & u, const Key & c, const RTree & t)
    : RTreeQueueTempl<N, Value>(c, t), u(u) {

        const typename RTree::Node * root = t.m_root;
        for(int i = 0; i < root->m_count; ++i) {
            push(root->m_branch[i]);
        }
    }
};

template<int N, typename Value> 
struct RTreeQueue : public RTreeQueueTempl<N, Value> {

    using RTree = typename RTreeQueueTempl<N, Value>::RTree;
    using Key = Rectangle<N>;


    void push(const typename RTree::Branch & branch) {
        Key aux(Point<N>(branch.m_rect.m_min), Point<N>(branch.m_rect.m_max));
        RTreeQueueTempl<N, Value>::queue.emplace(distance(aux, RTreeQueueTempl<N, Value>::center), &branch);
    }

    RTreeQueue(const Key & c, const RTree & t)
    : RTreeQueueTempl<N, Value>(c, t) {
        
        const typename RTree::Node * root = t.m_root;
        for(int i = 0; i < root->m_count; ++i) {
            push(root->m_branch[i]);
        }
    }
};
