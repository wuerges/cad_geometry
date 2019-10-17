#include <rapidcheck.h>

#include <ait.hpp>
#include <rtree_adapter.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <geometry.hpp>
#include <set>
#include <vector>

// #include <chrono>

// template<typename TimeT = std::chrono::milliseconds>
// struct measure
// {
//     template<typename F, typename ...Args>
//     static typename TimeT::rep execution(F&& func, Args&&... args)
//     {
//         auto start = std::chrono::steady_clock::now();
//         std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
//         auto duration = std::chrono::duration_cast< TimeT> 
//                             (std::chrono::steady_clock::now() - start);
//         return duration.count();
//     }
// };

#include "generators.hpp"

using namespace std;
using namespace geometry;


void test_collect_diamond(const Shape &center, vector<Shape> shapes, unsigned radius) {
    sort(shapes.begin(), shapes.end());
    shapes.erase(unique(shapes.begin(), shapes.end()), shapes.end());

    AIT ait;
    RTree rtree;
    auto start1 = std::chrono::steady_clock::now();
    ait.populate(shapes);
    // auto r_ait = ait.collect(center.a,center.b);
    auto r_ait = ait.collect_diamond(center, radius);


    auto start2 = std::chrono::steady_clock::now();
    rtree.populate(shapes);
    // auto r_rtree = rtree.collect(center.a,center.b);
    auto r_rtree = rtree.collect_diamond(center, radius);
    auto finish = std::chrono::steady_clock::now();

    auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds> (start2-start1);
    auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds> (finish-start2);

    RC_LOG() << "ait duration = " << duration1.count() << '\n';
    RC_LOG() << "rtree duration = " << duration2.count() << '\n';


    RC_LOG() << "shapes added = \n";
    for(auto s : shapes) {
        RC_LOG() << s << '\n';
    }

    RC_LOG() << "R_ait results = \n";
    for(auto r : r_ait) {
        RC_LOG() << r << '\n';
    }    
    RC_LOG() << "R_rtree results = \n";
    for(auto r : r_rtree) {
        RC_LOG() << r << '\n';
    }    

    RC_ASSERT(r_ait.size() == r_rtree.size());
}


int main(int n, char **argv) {
    rc::check("Check that collect_diamond has the same behavior", test_collect_diamond );
    return 0;
}