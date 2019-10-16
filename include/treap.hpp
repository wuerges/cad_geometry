#pragma once

#include <geometry.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <rtree_adapter.hpp>

namespace geometry {

/**
 * A facade for the Treap, with easier to use functions.
 */


struct Node;
struct AIT {

  void populate(const std::vector<Shape> &shapes);
  void add(const Shape &shape);
  bool hits(const PT l, const PT r) const;
  std::vector<Shape> collect(const PT l, const PT r) const;
  std::vector<Shape> neighboors_diamond(const Shape &u, size_t number) const;
  std::vector<Shape> collect_diamond(const Shape & center, int radius) const;
  std::vector<Shape> collect_diamond_2(const Shape & center, int radius1, int radius2) const;
  
  private:
  
  int query_diamond(const Shape & center, int radius) const;

  std::vector<Shape> collect(const PT center, int radius) const;
  std::vector<Shape> neighboors(const Shape &u, size_t number) const;
  std::vector<Shape> neighboors_sphere(const Shape &u, size_t number) const;
  std::unique_ptr<Node> root;

};
using Treap = RTree;

struct Node {
    Node(const Shape _x) : x(_x), low(min(_x.a, _x.b)), high(max(_x.a, _x.b)), count(1) {}


    void add(const Shape &shape, int level = 0);
    bool hits(const PT l, const PT r, int level = 0);
    int collect_diamond(std::vector<Shape> &results, const Shape & center, int radius,
                int level = 0);
    int collect_diamond_2(std::vector<Shape> &results, const Shape & center, 
            int radius1, int radius2,
                int level = 0);

    // Visits every shape that is close to the center shape, given a radius.
    // For each visited shape, apply f. If f returns false, stops the visit.
    template<typename Func >
    int visit_diamond(const Shape & center, int radius, Func f, int level = 0);
    
    template <typename Func>
    int visit_diamond_2(const Shape & center,
                         int radius1, int radius2, Func f, int level = 0);
    
    const Shape x;
    PT low, high;
    int count;

    std::unique_ptr<Node> left, right;
    // int query(const PT l, const PT r, int level = 0);
    int query_sphere(const PT center, int radius, int level = 0);
    int query_diamond(const Shape & center, int radius, int level = 0);
    // int query_diamond_2(const Shape & center, int radius1, int radius2, int level = 0);
    int collect(std::vector<Shape> &results, const PT l, const PT r,
        int level = 0);
    int collect_sphere(std::vector<Shape> &results, const PT center, int radius,
                int level = 0);
    void print(int h = 0, int level = 0);
};


} // namespace geometry