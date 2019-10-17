#include <rtree_adapter.hpp>

#include <set>

namespace geometry{


void RTree::populate(const std::vector<Shape> &param) {
    for (const Shape &s : param) {
        add(s);
    }
}

void RTree::add(const Shape &shape) {
    int id = shapes.size();
    shapes.push_back(shape);
    
    int v1[3], v2[3];
    to_intv(shape.a, v1);
    to_intv(shape.b, v2);
    
    tree.Insert(v1, v2, id);
}

int RTree::visit_diamond(const Shape & center, int radius, 
    const std::function <bool (const Shape &)>& f
    ) const {
    auto p = to_rect(center);    
    return tree.SearchDiamond(p.m_min, p.m_max, radius, [&f,this](int i)->bool { return f(shapes[i]); });
}

int RTree::visit(const Shape & center, 
    const std::function <bool (const Shape &)>& f) const {
    auto p = to_rect(center);    
    return tree.Search(p.m_min, p.m_max, [&f,this](int i) { return f(shapes[i]); });
}


bool RTree::hits(const PT l, const PT r) const {
    bool found = false;
    const Shape center{l, r};
    this->visit(center, [&found] (const Shape & x) ->bool {
        found = true;
        return false;
    });
    return found;    
}
std::vector<Shape> RTree::collect(const PT l, const PT r) const {
    std::vector<Shape> res;
    std::set<Shape> temp;
    
    const Shape center{l,r};
    visit(center, [&temp](const Shape & x) {
        temp.insert(x);
        return true;
    });
    for(auto & x : temp) { res.push_back(x); }
    return res;
}

std::vector<Shape> RTree::neighboors_diamond(const Shape &u, size_t number) const {

  std::vector<Shape> res;
  std::set<Shape> temp;

  int a = 0, b = 1;
  int nmax = 0;  
  while (temp.size() < number && nmax < 100) {
    visit_diamond(u, b, [&temp](const Shape & v) {
      temp.insert(v);
      return true;
    });
    a = b;
    b = b*2;
    nmax++;
  }
  for(auto & s : temp) { res.push_back(s); }
  return res;

}


std::vector<Shape> RTree::collect_diamond(const Shape & u, int radius) const {
    std::vector<Shape> res;
    visit_diamond(u, radius, [&res](const Shape & v) {
        res.push_back(v);
        return true;
    });
    return res; 

}
std::vector<Shape> RTree::collect_diamond_2(const Shape & u, int radius1, int radius2) const {
   std::vector<Shape> res;
    visit_diamond(u, radius2, [radius1, &u,&res](const Shape & v) {
        if(radius1==0  || distance(u, v) > radius1) {
            res.push_back(v);
        }
        return true;
    });
    return res; 
}

void to_intv(const PT & p, int* v) {
    v[0] = p.x;
    v[1] = p.y;
    v[2] = p.z;
}

RTree::MyTree::Rect to_rect(const Shape & s) {
    RTree::MyTree::Rect r;
    to_intv(s.a, r.m_min);    
    to_intv(s.b, r.m_max);
    return r;
}

}