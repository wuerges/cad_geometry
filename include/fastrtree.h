#include <geometry.hpp>
#include <array>
#include <optional>

namespace fastrtree {

using namespace geometry;
using std::array;
using std::optional;



template<int N, int MAX_DEGREE, typename T>
struct Branch;

template<int N, int MAX_DEGREE, typename T>
struct RTree {
    Rectangle<N> rect;
    T value;
    Branch<N, MAX_DEGREE, T> * branch;

    RTree(const Rectangle<N> & r, const T v):rect(r), value(v), branch(nullptr) {}


    optional<RTree*> insert_p(const Rectangle<N> & r, const T value) {
        if(branch) {
            
        }
        else {
            return new RTree(r, value);
        }
    }
};


template<int N, int MAX_DEGREE, typename T>
struct Branch {
  array<RTree<N, MAX_DEGREE, T>*, MAX_DEGREE> child;  
};





}