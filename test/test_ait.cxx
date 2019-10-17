#include <rapidcheck.h>

#include <ait.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <geometry.hpp>
#include <set>
#include <vector>

#include "generators.hpp"

using namespace std;
using namespace geometry;

void test_ait(const vector<Shape> &shapes) {
  if (shapes.size() == 0)
    return;
  unique_ptr<Node> root(new Node(shapes[0]));

  for (size_t i = 1; i < shapes.size(); i++) {
    root->add(shapes[i]);
  }

  // cout << root->count << '\n';
  // root->print();
  //

  for (const Shape &s : shapes) {
    bool count = root->hits(s.a, s.b);
    // cout << count << " ";
    // if(count > 1 && root->count < 10)  {
    //   cout << "-----------------\n";
    //   root->print();
    //   cout << "shape: " <<s<<" count: " << count<< "\n";
    // }
    RC_ASSERT(count);
  }
}

int test_ait_collect(const vector<Shape> &shapes) {
  if (shapes.size() == 0)
    return 0;
  unique_ptr<Node> root(new Node(shapes[0]));

  for (size_t i = 1; i < shapes.size(); i++) {
    root->add(shapes[i]);
  }

  for (const Shape &s : shapes) {
    vector<Shape> res;
    root->collect(res, s.a, s.b);

    for (const Shape &rs : res) {
      RC_ASSERT(collides(rs, s));
      RC_ASSERT(root->hits(s.a, s.b));
    }
  }

  return 0;
}

void test_ait_collect_diamond(const Shape &center, const vector<Shape> &shapes, unsigned radius) {

    AIT ait;
    ait.populate(shapes);

    // radius = std::min(radius, 10000u);

    auto res = ait.collect_diamond(center, radius);  

  
    set<Shape> shapeset;
    shapeset.insert(res.begin(), res.end());

    for (const Shape &s : shapes) {
        RC_LOG() << "------------------\n";
        RC_LOG() << "shape=" << s << "\n";
        RC_LOG() << "  distance=" << distance(center, s) << "\n";
        RC_LOG() << "  radius=" << radius << '\n';
        RC_LOG() << "  found ? " << (shapeset.find(s) != shapeset.end() ? "true" : "false") << '\n';
    }
    

    for (const Shape &s : shapes) {
        if(distance(center, s) <= radius) {            
            RC_ASSERT(shapeset.find(s) != shapeset.end());
        }
        else {
            RC_ASSERT(shapeset.find(s) == shapeset.end());
        }
    }

}

int test_ait_collect_neg(const vector<Shape> &shapes) {
  if (shapes.size() == 0) {
    RC_DISCARD("discarding empty testcase");
    return 0;
  }

  unique_ptr<Node> root(new Node(shapes[0]));

  for (size_t i = 1; i < shapes.size(); i++) {
    root->add(shapes[i]);
  }

  for (const Shape &s : shapes) {
    set<Shape> everything;
    for (auto &si : shapes) {
      everything.insert(si);
    }

    vector<Shape> res;
    root->collect(res, s.a, s.b);
    if (res.size() == 0) {
      RC_DISCARD("discarding empty testcase");
    }

    RC_LOG() << "\n\n// Query\n" << s << '\n';

    RC_LOG() << "// collected shapes \n";
    for (const Shape &rs : res) {
      RC_LOG() << rs << '\n';
      everything.erase(rs);
    }

    RC_LOG() << "// uncollected shapes \n";
    for (const Shape &si : everything) {
      RC_LOG() << si << '\n';
    }

    for (const Shape &si : everything) {
      RC_ASSERT(!collides(s, si));
    }
  }
  //
  // cout << "// Unmatched shapes"
  // for(const Shape & s : everything) {
  //
  //   RC_LOG() << s << '\n';
  // }
  //
  //

  return 0;
}

int main(int n, char **argv) {

  rc::check("Check that added shapes can be queried",
            [](const std::vector<Shape> &shapes) { test_ait(shapes); });

  rc::check(
      "Check that collected shapes are colliding with the query",
      [](const std::vector<Shape> &shapes) { test_ait_collect(shapes); });

  rc::check(
      "Check that shapes that were not collected do not collide with the query",
      [](const std::vector<Shape> &shapes) { test_ait_collect_neg(shapes); });

  rc::check("Check ait_collect_diamond.", test_ait_collect_diamond);

  // vector<Shape> shapes;
  //
  // shapes.push_back(Shape{PT{10, 10, 0}, PT{20, 20, 0}});
  // shapes.push_back(Shape{PT{30, 30, 0}, PT{40, 40, 0}});
  // shapes.push_back(Shape{PT{10, 10, 0}, PT{40, 40, 0}});
  // shapes.push_back(Shape{PT{0, 0, 0}, PT{20, 20, 0}});
  // shapes.push_back(Shape{PT{10, 10, 0}, PT{50, 50, 0}});
  // test_ait(shapes);

  return 0;
}