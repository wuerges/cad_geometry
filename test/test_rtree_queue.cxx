#include <rtree_queue.hpp>
#include <RTree.h>
#include <rapidcheck.h>

#include "generators.hpp"

using namespace geometry;
using namespace std;


void test_rtree_collect_diamond(const Shape & center, const vector<Shape> &shapes) {

    auto f =[&]() {
        rtree::RTree<const Shape*, int, 3, double> t;
        for(const Shape & s : shapes) {
            t.Insert(s.p1.coords.begin(), s.p2.coords.begin(), &s);
        }
        return t;        
    };

    const auto tree = f();

    // RTreeQueue<3, Shape> queue(s, tree);
    RTreeQueue<3, const Shape> queue(center, tree);


}


int main () {


    return 0;


}
