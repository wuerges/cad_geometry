#pragma once

#include <array>
#include <rapidcheck.h>


using namespace std;
using namespace geometry;

namespace rc {


template <int NDIM>
const auto genPT = gen::container<array<int, NDIM>>(gen::inRange(0, 100000));

template<int NDIM>
struct Arbitrary<PT<NDIM>> {
    static Gen<PT<NDIM>> arbitrary() {
    return gen::map(genPT<NDIM>,
        [](array<int, NDIM> vs) {
            return PT<NDIM>(vs);
        });
    }
};

template<int NDIM>
struct Arbitrary<Shape<NDIM> > {
    static Gen<Shape<NDIM> > arbitrary() {
    return gen::map(gen::container<array<PT<3>, 2> >(gen::arbitrary<PT<3>>()),
        [](const array<PT<3>, 2> pts) {
            return Shape<NDIM>(pts[0], pts[1]);
        });
    }
};

}