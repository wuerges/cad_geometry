#include <rtree_adapter.hpp>

using namespace geometry;
using namespace std;

int main () {
    return 0;

    RTree tree;

    Shape s1;

    tree.visit_diamond(s1, 100, [](const Shape & s){ cout << "visited " << s << '\n'; return true; });
}