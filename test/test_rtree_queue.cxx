// #include <rtree_queue.hpp>
#include <rtree_adapter.hpp>
#include <RTree.h>
#include <rapidcheck.h>

#include "generators.hpp"

using namespace geometry;
using namespace std;


void test_rtree_queue(const Shape & center, vector<Shape> shapes) {

    // auto f =[&]() {
    //     rtree::RTree<const Shape*, int, 3, double> t;
    //     for(const Shape & s : shapes) {
    //         t.Insert(s.p1.coords.begin(), s.p2.coords.begin(), &s);
    //     }
    //     return t;        
    // };
    // const auto tree = f();
    // RTreeQueue<3, Shape> queue(s, tree);

    
    RTree tree;
    tree.populate(shapes);
    RTreeQueue queue(center, tree);


    sort(shapes.begin(), shapes.end(), [&](auto s1, auto s2) {
        return distance(s1, center) < distance(s2, center);
    });





    vector<Shape> result;
    while(!queue.empty()) {
        result.push_back(*queue.pop());
    }
    RC_LOG() << "Results: ";
    for(auto r : result) {
        RC_LOG() << distance(center, r) << ":";
        RC_LOG() << r << ' ';
    }
    RC_LOG() << "\n";
    RC_LOG() << "Shapes: ";
    for(auto r : shapes) {
        RC_LOG() << r << ' ';
    }
    RC_LOG() << "\n";

    RC_ASSERT(result.size() == shapes.size());

    for(int i = 0; i < shapes.size(); ++i) {
        RC_ASSERT(distance(center, shapes[i]) == distance(center, result[i]));
    }
}


int main () {

    rc::check("Testing RTree queue without filter", test_rtree_queue);

    return 0;


}
