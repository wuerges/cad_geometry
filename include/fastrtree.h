#include <geometry.hpp>
#include <array>
#include <optional>

namespace fastrtree {

using namespace geometry;
using std::array;
using std::vector;
using std::optional;
using std::pair;



template<int N, int MAX_DEGREE, typename T>
struct Branch;

template<int N, int MAX_DEGREE, typename T>
struct RTree {
    Rectangle<N> rect;
    T value;
    vector<RTree<N, MAX_DEGREE, T>> * child;

    RTree(const Rectangle<N> & r, const T v):rect(r), value(v), child(nullptr) {}
    RTree(const Rectangle<N> & r):rect(r), child(new vector<RTree<N, MAX_DEGREE, T>>()) {}

    optional<RTree<N, MAX_DEGREE, T>> insert_p(const Rectangle<N> & r, const T value) {
        if(child) {
            RTree* sel = select(r);
            auto extra = sel->insert_p(r, value);
            if(extra) {
                child->push_back(*extra);
            }
            if(child->size() == MAX_DEGREE) {
                return split();
            }
            else {
                return std::nullopt;
            }
        }
        else {
            return RTree(r, value);
        }
    }

    RTree<N, MAX_DEGREE, T> * select(const Rectangle<N> & r) {
        
        RTree<N, MAX_DEGREE, T> * res = &*child->begin();
        double res_a = minimumBound(r, res->rect).area() - res->rect.area();

        for(auto & t : *child) {
            double cand_a = minimumBound(r, t.rect).area() - t.rect.area();
            if(cand_a < res_a) {
                res_a = cand_a;
                res = &t;
            }
        }
        return res;
    }

    RTree<N, MAX_DEGREE, T> split() {

        array<Rectangle<N>, 1<<MAX_DEGREE> mbrs;

        for(int i = 0; i < MAX_DEGREE; ++i) {
            mbrs[1<<i] = (*child)[i].rect;
        }

        for(int i = 1; i < (1<<MAX_DEGREE); ++i) {
            int lb = (i & (i-1));
            if( (i & (i-1)) != 0) {
                mbrs[i] = minimumBound(mbrs[i-lb], mbrs[lb]);
            }
        }

        double min_a = 1e9;
        int min_set = 0;

        int all_sets = (1<<MAX_DEGREE) - 1;

        for(int i = 1; i < all_sets; ++i) {

            auto isect = intersection(mbrs[i], mbrs[all_sets - i]);
            double isect_a = isect ? isect->area() : 0;
            if(isect_a < min_a) {
                min_set = i;                
            }
        }

        RTree<N, MAX_DEGREE, T> ret(mbrs[min_set]);
        return ret;
    }
};

}