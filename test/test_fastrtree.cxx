#include <fastrtree.h>
#include <rapidcheck.h>

#include <geometry.hpp>

#include "generators.hpp"

using namespace geometry;
using namespace std;


int main () {

    Rectangle<3> r1;

    fastrtree::RTree<3, 4, int> tree(r1, 0);

    tree.insert_p(r1, 1);

    return 0;
}
