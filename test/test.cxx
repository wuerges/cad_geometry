#include <iostream>
#include <geometry.hpp>
#include <rapidcheck.h>
#include <iostream>

#include "generators.hpp"

using namespace std;
using namespace geometry;


template<int NDIM>
void test_distance(const Shape<NDIM> & s1, const Shape<NDIM> & s2) {
    RC_LOG() << "distance is " << distance(s1, s2) << '\n';
    RC_LOG() << "collides? " << collides(s1, s2) << '\n';
    
    if(collides(s1, s2)) {  
        RC_ASSERT(distance(s1, s2) == 0);
    }    
}


template<int NDIM>
void test_distance_comutative(const Shape<NDIM> & s1, const Shape<NDIM> & s2) {
    RC_LOG() << "distance is " << distance(s1, s2) << '\n';
    
    RC_ASSERT(distance(s1, s2) == distance(s2, s1));
}

int main(int argc, char ** argv) {

    // PT a(10, 10, 0);
    // PT b(20, 20, 0);
    // PT c(30, 30, 0);
    // PT d(15, 15, 0);
    // PT e(0, 0, 0);
    // PT f(0, 0, 1);

    // Shape a(e,e);
    // cout <<"distance " << distance(a, a) << '\n';

    
    // cout << "Distance " << distance(c, Shape(a, b)) << '\n';
    // // assert(distance(c, Shape(a, b)) == 21);
    
    // cout << "Distance " << distance(d, Shape(a, b)) << '\n';
    // // assert(distance(e, Shape(a, b)) == 1);

    // cout << "Distance " << distance(e, Shape(a, b)) << '\n';
    // // RC_ASSERT(distance(e, Shape(a, b)) == 21);

    
    // cout << "Distance " << distance(c, Shape(a, c)) << '\n';
    // // assert(distance(c, Shape(a, b)) == 21);
    
    // cout << "Distance " << distance(d, Shape(a, c)) << '\n';
    // // assert(distance(e, Shape(a, b)) == 1);

    // cout << "Distance " << distance(e, Shape(a, c)) << '\n';
    // // RC_ASSERT(distance(e, Shape(a, b)) == 21);


    // cout << "Expand " << Shape(a, b) << " by 3 = " << Shape(a, b).expand(3) << "\n";


    rc::check("Check that if shapes collide, their distance is 0", test_distance<3>);
    rc::check("Check that distance is commutative", test_distance_comutative<3>);

    return 0;
}

