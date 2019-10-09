#pragma once

#include <geometry.hpp>
#include <RTree.h>

#include <vector>


namespace geometry {

using std::vector;


void to_intv(const PT & p, int* v) {
    v[0] = p.x;
    v[1] = p.y;
    v[2] = p.z;
}

class RTree {
    using MyTree = rtree::RTree<int, int, 3, double>;

    MyTree tree;
    vector<Shape> shapes;

public:
    void populate(const std::vector<Shape> &param) {
        for (const Shape &s : param) {
            add(s);
        }
    }

    void add(const Shape &shape) {
        int id = shapes.size();
        shapes.push_back(shape);
        
        int v1[3], v2[3];
        to_intv(shape.a, v1);
        to_intv(shape.b, v2);
        
        tree.Insert(v1, v2, id);
    }

    template<typename Func >
    int visit_diamond(const Shape & center, int radius, Func f) {
        int v1[3], v2[3];
        to_intv(center.a, v1);
        to_intv(center.b, v2);        
        return tree.SearchDiamond(v1, v2, radius, f);
    }
    
    // template <typename Func>
    // int visit_diamond_2(const Shape & center,
    //                      int radius1, int radius2, Func f);

    // int query(const PT l, const PT r) const {
    //     if (root) {
    //     return root->query(min(l, r), max(l, r));
    //     }
    //     return 0;
    // }

//   int query(const PT center, int radius) const {
//     if (root) {
//       return root->query_sphere(center, radius);
//     }
//     return 0;
//   }

//   int query_diamond(const Shape & center, int radius) const {
//     if (root) {
//       return root->query_diamond(center, radius);
//     }
//     return 0;
//   }

//   int hits(const PT l, const PT r) const {
//     if (root) {
//       return root->hits(min(l, r), max(l, r));
//     }
//     return 0;
//   }


//   std::vector<Shape> collect(const PT l, const PT r) const {

//     std::vector<Shape> results;
//     if (root) {
//       root->collect(results, l, r);
//     }
//     return results;
//   }

//   std::vector<Shape> collect(const PT center, int radius) const {

//     std::vector<Shape> results;
//     if (root) {
//       root->collect_sphere(results, center, radius);
//     }
//     return results;
//   }

//   std::vector<Shape> collect_diamond(const Shape & center, int radius) const {

//     std::vector<Shape> results;
//     if (root) {
//       root->collect_diamond(results, center, radius);
//     }
//     return results;
//   }

//   std::vector<Shape> collect_diamond_2(const Shape & center, int radius1, int radius2) const {

//     std::vector<Shape> results;
//     if (root) {
//       root->collect_diamond_2(results, center, radius1, radius2);
//     }
//     return results;
//   }

//   std::vector<Shape> neighboors(const Shape &u, size_t number) const {
//     int w = 1;
//     PT l = u.a;
//     PT r = u.b;
//     int q = query(PT(l.x - w, l.y - w, l.z - w), PT(r.x + w, r.y + w, r.z + w));
//     // std::cout << " w =" << w << "\n";
//     while (q <= number && w < 1e8) {
//       w = w * 2;
//       q = query(PT(l.x - w, l.y - w, l.z - w), PT(r.x + w, r.y + w, r.z + w));
//       // std::cout << "Query=" << q << " w=" << w
//       //   << " pts=" << PT(l.x-w, l.y-w, l.z-w)
//       //   << " <-> "<<  PT(r.x+w, r.y+w, r.z+w) <<  '\n';
//     }
//     return collect(PT(l.x - w, l.y - w, l.z - w),
//                    PT(l.x + w, l.y + w, l.z + w));
//   }

//   std::vector<Shape> neighboors_sphere(const Shape &u, size_t number) const {
//     int w = 1;
//     PT center =
//         PT{(u.a.x + u.b.x) / 2, (u.a.y + u.b.y) / 2, (u.a.z + u.b.z) / 2};

//     int t = abs(u.a.x - u.b.x) + abs(u.a.y-u.b.y) + abs(u.a.z-u.b.z);
//     int q = query(center, t+w);
//     // std::cout << " w =" << w << "\n";
//     while (q <= number && w < 1e8) {
//       w = w * 2;
//       q = query(center, t+w);
//       // std::cout << "Query=" << q << " w=" << w << '\n';
//       // std::cout << "Query=" << q << " w=" << w
//       //   << " pts=" << PT(l.x-w, l.y-w, l.z-w)
//       //   << " <-> "<<  PT(r.x+w, r.y+w, r.z+w) <<  '\n';
//     }
//     return collect(center, t+w);
//   }

//   std::vector<Shape> neighboors_diamond(const Shape &u, size_t number) const {
//     int base_number = query(u.a, u.b);
//     int mh = manhatan(u.a, u.b);
//     int w = 1;
//     int q = query_diamond(u, w);
//     // std::cout << " w =" << w << "\n";
//     // return collect_diamond(center, t+8000);

//     // while (q <= number && w < 1e8) {
//     while (q <= number+base_number && w < 1e8) {
//       w *= 2;
//       q = query_diamond(u, w);
//       // std::cout << "Query=" << q << " w=" << w << '\n';
//       // std::cout << "Query=" << q << " w=" << w
//       //   << " pts=" << PT(l.x-w, l.y-w, l.z-w)
//       //   << " <-> "<<  PT(r.x+w, r.y+w, r.z+w) <<  '\n';
//     }
//     return collect_diamond(u, w);
//   }
    
};

}