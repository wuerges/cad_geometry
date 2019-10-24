#include <rtree_adapter.hpp>
#include <RTree.h>
#include <rapidcheck.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <geometry.hpp>
#include <set>
#include <vector>

#include "generators.hpp"

using namespace geometry;
using namespace std;


// RTree::MyTree::Rect to_rect(const Shape & s) {
//     RTree::MyTree::Rect r;
//     to_intv(s.a, r.m_min);    
//     to_intv(s.b, r.m_max);
//     return r;
// }

void test_rtree_distance(const Shape &a, const Shape &b) {

    RTree rtree;

    RTree::MyTree::Rect ra = to_rect(a), rb = to_rect(b);
    
    RC_LOG() << "RA={a=" << ra.m_min[0] << ' ' << ra.m_min[1] << ' '  << ra.m_min[2] << "}{b=" << ra.m_max[0] << ' ' << ra.m_max[1] << ' '  << ra.m_max[2] << "}\n";
    
    RC_LOG() << "RB={a=" << rb.m_min[0] << ' ' << rb.m_min[1] << ' '  << rb.m_min[2] << "}{b=" << rb.m_max[0] << ' ' << rb.m_max[1] << ' '  << rb.m_max[2] << "}\n";
    
    RC_ASSERT(distance(a, b) == rtree.tree.distance(&ra, &rb));
}


void test_rtree_collect(vector<Shape> shapes) {
    sort(shapes.begin(), shapes.end());
    shapes.erase(unique(shapes.begin(), shapes.end()), shapes.end());


    RTree rtree;
    rtree.populate(shapes);

    for (const Shape &s : shapes) {
        auto res = rtree.collect(s.a, s.b);
        set<Shape> shapeset;
        shapeset.insert(res.begin(), res.end());

        RC_ASSERT(shapeset.find(s) != shapeset.end());

        for(const Shape &z : shapes) {
            RC_LOG() << "\n-------- error ----------\n";
            RC_LOG() << "center=" << s << "\n";
            RC_LOG() << "shape=" << z << "\n";
            RC_LOG() << "  distance=" << distance(s, z) << "\n";
            RC_LOG() << "  found ? " << (shapeset.find(s) != shapeset.end() ? "true" : "false") << '\n';
            
            if(collides(s, z) ) {
                RC_ASSERT(shapeset.find(z) != shapeset.end());
            }
            else {
                RC_ASSERT(shapeset.find(z) == shapeset.end());
            }
        }
    }   
    
}


void test_rtree_collect_diamond(vector<Shape> shapes, unsigned radius) {
    sort(shapes.begin(), shapes.end());
    shapes.erase(unique(shapes.begin(), shapes.end()), shapes.end());


    RTree rtree;
    rtree.populate(shapes);

    // radius = std::min(radius, 10000u);
  

    for (const Shape &s : shapes) {
        auto res = rtree.collect_diamond(s, radius);
        set<Shape> shapeset;
        shapeset.insert(res.begin(), res.end());

        RC_ASSERT(shapeset.find(s) != shapeset.end());

        for(const Shape &z : shapes) {
            RC_LOG() << "\n------------------\n";
            RC_LOG() << "center=" << s << "\n";
            RC_LOG() << "shape=" << z << "\n";
            RC_LOG() << "  distance=" << distance(s, z) << "\n";
            RC_LOG() << "  found ? " << (shapeset.find(z) != shapeset.end() ? "true" : "false") << '\n';
            
            if(distance(s, z) <= radius) {
                RC_ASSERT(shapeset.find(z) != shapeset.end());
            }
            else {
                RC_ASSERT(shapeset.find(z) == shapeset.end());
            }
        }
    }   
    
}




// void test_rtree_collect_diamond(const Shape &center, vector<Shape> shapes, unsigned radius) {
//     sort(shapes.begin(), shapes.end());
//     shapes.erase(unique(shapes.begin(), shapes.end()), shapes.end());


//     RTree rtree;
//     rtree.populate(shapes);

//     // radius = std::min(radius, 10000u);

//     auto res = rtree.collect_diamond(center, radius);  

  
//     set<Shape> shapeset;
//     shapeset.insert(res.begin(), res.end());

//     for (const Shape &s : shapes) {
//         RC_LOG() << "------------------\n";
//         RC_LOG() << "shape=" << s << "\n";
//         RC_LOG() << "  distance=" << distance(center, s) << "\n";
//         RC_LOG() << "  radius=" << radius << '\n';
//         RC_LOG() << "  found ? " << (shapeset.find(s) != shapeset.end() ? "true" : "false") << '\n';
//     }
    
//     bool fail = false;

//     for (const Shape &s : shapes) {
//         auto rc = to_rect(center), rs = to_rect(s);
//         RC_LOG() << "\n-------- error ----------\n";
//         RC_LOG() << "center=" << center << "\n";
//         RC_LOG() << "shape=" << s << "\n";
//         RC_LOG() << "  distance=" << distance(center, s) << "\n";
//         RC_LOG() << "  radius=" << radius << '\n';
//         RC_LOG() << "  found ? " << (shapeset.find(s) != shapeset.end() ? "true" : "false") << '\n';
//         RC_LOG() << "  Overlap? " << (rtree.tree.OverlapDiamond(&rc, &rs, radius)) << '\n';

//         if(distance(center, s) <= radius) {            
//             // if(shapeset.find(s) == shapeset.end()) {
//             //     fail = true;
//             // }
            
//             RC_ASSERT(shapeset.find(s) != shapeset.end());
//         }
//         else {
//             // if(shapeset.find(s) != shapeset.end()) {
//             //     fail = true;
//             // }
//             RC_ASSERT(shapeset.find(s) == shapeset.end());
//         }
//     }
//     // RC_ASSERT(!fail);

// }


int main () {

    rc::check("Test RTree.h distance.", test_rtree_distance);
    rc::check("Test rtree collect.", test_rtree_collect);

    rc::check("Check rtree_collect_diamond.", test_rtree_collect_diamond);
    

    return 0;


}
