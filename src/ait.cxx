#include <geometry.hpp>
#include <treap.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <future>

namespace geometry {

using std::unique_ptr, std::make_unique, std::cout;

void Node::add(const Shape &shape, int level) {
  int a = x.a[level % 3];
  int b = x.b[level % 3];
  int l = shape.a[level % 3];
  int r = shape.b[level % 3];
  if (r < a) {
    if (left) {
      left->add(shape, level + 1);
    } else {
      left = make_unique<Node>(shape);
    }
  } else {
    if (right) {
      right->add(shape, level + 1);
    } else {
      right = make_unique<Node>(shape);
    }
  }
  if (left) {
    low = min(low, left->low);
    high = max(high, left->high);
  }
  if (right) {
    low = min(low, right->low);
    high = max(high, right->high);
  }
  count = 1 + (left ? left->count : 0) + (right ? right->count : 0);
}


int Node::query_sphere(const PT center, unsigned radius, int level) {
  // std::cout << "query_sphere("<<center<<","<<radius<<","<<level<<")\n";

  // Passes for 100.000 tests
  if (  ((center[level % 3] - radius) > high[level % 3])
     || ((center[level % 3] + radius) < low[level % 3])   ) {
    return 0;
  }

  // Passes for 100.000 tests
  if (sphere_contains(center, radius, low, high)) {
    // std::cout << "sphere " << center << " " << radius << '\n';
    // std::cout << "case contains " << low << " " << high << '\n';
    // std::cout << "count = " << count << '\n';
    return count;
  }
  // if (!sphere_collides(center, radius, low, high)) {
  //   return 0;
  // }
  bool hits = sphere_collides(center, radius, x.a, x.b);

  return (hits ? 1 : 0) +
         (left ? left->query_sphere(center, radius, level + 1) : 0) +
         (right ? right->query_sphere(center, radius, level + 1) : 0);
}

int Node::query_diamond(const Shape & center, unsigned radius, int level) {
  // std::cout << "query_sphere("<<center<<","<<radius<<","<<level<<")\n";

  // Passes for 100.000 tests
  if (  ((center.a[level % 3] - radius) > high[level % 3])
     || ((center.b[level % 3] + radius) < low[level % 3])   ) {
    return 0;
  }

  // Passes for 100.000 tests
  if (diamond_contains(center, radius, low, high)) {
    // std::cout << "sphere " << center << " " << radius << '\n';
    // std::cout << "case contains " << low << " " << high << '\n';
    // std::cout << "count = " << count << '\n';
    return count;
  }
  // if (!sphere_collides(center, radius, low, high)) {
  //   return 0;
  // }
  bool hits = diamond_collides(center, radius, x.a, x.b);

  return (hits ? 1 : 0) +
         (left ? left->query_diamond(center, radius, level + 1) : 0) +
         (right ? right->query_diamond(center, radius, level + 1) : 0);
}



// int Node::query(const PT l, const PT r, int level) {

//   if (l <= low && r >= high) {
//     return count;
//   }
//   if (l > high || r < low) {
//     return 0;
//   }
//   if (l[level % 3] > high[level % 3] || r[level % 3] < low[level % 3]) {
//     return 0;
//   }

//   bool hits = collides(x, Shape{l, r});

//   return (hits ? 1 : 0) + (left ? left->query(l, r, level + 1) : 0) +
//          (right ? right->query(l, r, level + 1) : 0);
// }

bool Node::hits(const PT l, const PT r, int level) {
  // std::cout << "hits: "<< level<<"\n";

  if (l <= low && r >= high) {
    // std::cout << "case1\n";
    return count > 0;
  }
  if (l > high || r < low) {
    // std::cout << "case2\n";
    return false;
  }
  if (l[level % 3] > high[level % 3] || r[level % 3] < low[level % 3]) {
    // std::cout << "case3\n";
    return false;
  }

  bool hits = collides(x, Shape{l, r});
  return hits ||
      (left ? left->hits(l, r, level + 1) : false) ||
      (right ? right->hits(l, r, level + 1) : false);

}

int Node::collect(std::vector<Shape> &results, const PT l, const PT r,
                  int level) {

  if (l > high || r < low) {
    return 0;
  }
  if (l[level % 3] > high[level % 3] || r[level % 3] < low[level % 3]) {
    return 0;
  }

  bool hits = collides(x, Shape{l, r});
  if (hits)
    results.push_back(x);

  return (hits ? 1 : 0) + (left ? left->collect(results, l, r, level + 1) : 0) +
         (right ? right->collect(results, l, r, level + 1) : 0);
}

int Node::collect_sphere(std::vector<Shape> &results, const PT center,
                         unsigned radius, int level) {
  if (  ((center[level % 3] - radius) > high[level % 3])
     || ((center[level % 3] + radius) < low[level % 3])   ) {
    return 0;
  }



  // std::cout << "collect_sphere\n";
  bool hits = sphere_collides(center, radius, x.a, x.b);
  // std::cout << "center: " << center << " radius = " << radius << " (" << x << ")\n";
  if (hits) {
    // std::cout << "HIT! " << x <<'\n';
    results.push_back(x);
  }


  // if (!sphere_collides(center, radius, low, high)) {
  //   return 0;
  // }

  return (hits ? 1 : 0) +
         (left ? left->collect_sphere(results, center, radius, level + 1) : 0) +
         (right ? right->collect_sphere(results, center, radius, level + 1)
                : 0);
}

int Node::collect_diamond(std::vector<Shape> &results, const Shape & center,
                         unsigned radius, int level) {

    return visit_diamond(
        center, 
        radius, 
        [&results](const Shape & s)->bool { 
            results.push_back(s);
            return true; 
            },
            level);
}

// Visits every shape that is close to the center shape, given a radius.
    // For each visited shape, apply f. If f returns false, stops the visit.
int Node::visit_diamond(const Shape & center, unsigned radius, 
  const std::function <bool (const Shape &)>& f, int level) {
    if (  ((center.a[level % 3] - radius) > high[level % 3])
        || ((center.b[level % 3] + radius) < low[level % 3]) ) {
        return 0;
    }



    bool hits = diamond_collides(center, radius, x.a, x.b);
    if (hits) {
        if (!f(x)) return 0;
    }

    return (hits ? 1 : 0) +
            (left ? left->visit_diamond(center, radius, f, level + 1) : 0) +
            (right ? right->visit_diamond(center, radius, f, level + 1)
            : 0);
    
}

int Node::collect_diamond_2(std::vector<Shape> &results, const Shape & center,
                         unsigned radius1, unsigned radius2, int level) {
    return visit_diamond_2(
        center, 
        radius1, 
        radius2, 
        [&results](const Shape & s)->bool { 
            results.push_back(s);
            return true; 
            },
            level);
}

int Node::visit_diamond_2(const Shape & center,
                         unsigned radius1, unsigned radius2, 
                         const std::function <bool (const Shape &)>& f, int level) {
    if (  ((center.a[level % 3] - radius2) > high[level % 3])
        || ((center.b[level % 3] + radius2) < low[level % 3])   
    ) {
        return 0;
    }

    if(radius1 > 0 && diamond_contains(center,radius1-1, low, high) ) return 0;
    // bool hits1 = radius1 == 0 || !diamond_collides(center, radius1, x.a, x.b);
    bool hits1 = radius1 > 0 && diamond_collides(center, radius1, x.a, x.b);
    bool hits = diamond_collides(center, radius2, x.a, x.b);

    if (!hits1 && hits) {
        if (!f(x)) return 0;
    }

    return (hits ? 1 : 0) +
            (left ? left->visit_diamond_2(center, radius1, radius2, f, level + 1) : 0) +
            (right ? right->visit_diamond_2(center, radius1, radius2, f, level + 1)
            : 0);
}

void Node::print(int h, int level) {
  int a = x.a[level % 3];
  int b = x.b[level % 3];

  if (right) {
    // for(int i = 0; i < h; ++i) { cout << ' '; }
    // cout << "r\n";
    right->print(h + 2, level + 1);
  }
  for (int i = 0; i < h; ++i) {
    cout << ' ';
  }
  printf("%d:{{(%d, %d, %d), (%d , %d, %d)} (%d, %d, %d) (%d, %d, %d) %d}\n",
         level, x.a[0], x.a[1], x.a[2], x.b[0], x.b[1], x.b[2], low[0], low[1],
         low[2], high[0], high[1], high[2], count);
  if (left) {
    // for(int i = 0; i < h; ++i) { cout << ' '; }
    // cout << "l\n";
    left->print(h + 4, level + 1);
  }
}

std::vector<Shape> AIT::neighboors_diamond(const Shape &u, size_t number) const {

  std::vector<Shape> res;
  
  int a = 0, b = 1;
  while (res.size() < number) {
    root->visit_diamond_2(u, a, b, [&res](const Shape & v) {
      res.push_back(v);
      return true;
    });
    a = b;
    b = b*2;
  }
  
  return res;
}


  // std::vector<Shape> AIT::neighboors_sphere(const Shape &u, size_t number) const {
  //   int w = 1;
  //   PT center =
  //       PT{(u.a.x + u.b.x) / 2, (u.a.y + u.b.y) / 2, (u.a.z + u.b.z) / 2};

  //   int t = abs(u.a.x - u.b.x) + abs(u.a.y-u.b.y) + abs(u.a.z-u.b.z);
  //   int q = query(center, t+w);
  //   // std::cout << " w =" << w << "\n";
  //   while (q <= number && w < 1e8) {
  //     w = w * 2;
  //     q = query(center, t+w);
  //     // std::cout << "Query=" << q << " w=" << w << '\n';
  //     // std::cout << "Query=" << q << " w=" << w
  //     //   << " pts=" << PT(l.x-w, l.y-w, l.z-w)
  //     //   << " <-> "<<  PT(r.x+w, r.y+w, r.z+w) <<  '\n';
  //   }
  //   return collect(center, t+w);
  // }


  void AIT::populate(const std::vector<Shape> &shapes) {
    for (const Shape &s : shapes) {
      add(s);
    }
  }

  void AIT::add(const Shape &shape) {
    if (root) {
      root->add(shape);
    } else {
      root = std::make_unique<Node>(shape);
    }
  }


  // int AIT::query(const PT l, const PT r) const {
  //   if (root) {
  //     return root->query(min(l, r), max(l, r));
  //   }
  //   return 0;
  // }

  // int AIT::query(const PT center, unsigned radius) const {
  //   if (root) {
  //     return root->query_sphere(center, radius);
  //   }
  //   return 0;
  // }

  int AIT::query_diamond(const Shape & center, unsigned radius) const {
    if (root) {
      return root->query_diamond(center, radius);
    }
    return 0;
  }

  bool AIT::hits(const PT l, const PT r) const {
    if (root) {
      return root->hits(min(l, r), max(l, r));
    }
    return 0;
  }


  std::vector<Shape> AIT::collect(const PT l, const PT r) const {

    std::vector<Shape> results;
    if (root) {
      root->collect(results, l, r);
    }
    return results;
  }

  std::vector<Shape> AIT::collect(const PT center, unsigned radius) const {

    std::vector<Shape> results;
    if (root) {
      root->collect_sphere(results, center, radius);
    }
    return results;
  }

  std::vector<Shape> AIT::collect_diamond(const Shape & center, unsigned radius) const {

    std::vector<Shape> results;
    if (root) {
      root->collect_diamond(results, center, radius);
    }
    return results;
  }

  std::vector<Shape> AIT::collect_diamond_2(const Shape & center, unsigned radius1, unsigned radius2) const {

    std::vector<Shape> results;
    if (root) {
      root->collect_diamond_2(results, center, radius1, radius2);
    }
    return results;
  }

  // std::vector<Shape> AIT::neighboors(const Shape &u, size_t number) const {
  //   int w = 1;
  //   PT l = u.a;
  //   PT r = u.b;
  //   int q = query(PT(l.x - w, l.y - w, l.z - w), PT(r.x + w, r.y + w, r.z + w));
  //   // std::cout << " w =" << w << "\n";
  //   while (q <= number && w < 1e8) {
  //     w = w * 2;
  //     q = query(PT(l.x - w, l.y - w, l.z - w), PT(r.x + w, r.y + w, r.z + w));
  //     // std::cout << "Query=" << q << " w=" << w
  //     //   << " pts=" << PT(l.x-w, l.y-w, l.z-w)
  //     //   << " <-> "<<  PT(r.x+w, r.y+w, r.z+w) <<  '\n';
  //   }
  //   return collect(PT(l.x - w, l.y - w, l.z - w),
  //                  PT(l.x + w, l.y + w, l.z + w));
  // }


} // namespace iccad