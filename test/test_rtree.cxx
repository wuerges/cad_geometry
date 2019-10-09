#include <rtree_adapter.hpp>

using namespace geometry;
using namespace std;

int main () {

    cout << "\n---------------\nend\n";
    RTree tree;

    Shape s1(PT(0,0,0), PT(0,0,0));

    tree.add(s1);

    tree.visit_diamond(s1, 100, [](const Shape & s){ cout << "visited " << s << '\n'; return true; });

    return 0;


}