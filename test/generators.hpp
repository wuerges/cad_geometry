#pragma once

#include <array>
#include <rapidcheck.h>


using namespace std;
using namespace geometry;

namespace rc {



  const auto genPT = gen::container<array<int, 3>>(gen::inRange(0, 100000));

  template<>
  struct Arbitrary<PT> {
    static Gen<PT> arbitrary() {
      return gen::map(genPT,
        [](array<int, 3> vs) {
          return PT(vs.begin());
        });
    }
  };

  template<>
  struct Arbitrary<Shape> {
    static Gen<Shape> arbitrary() {
      return gen::map(gen::container<array<PT, 2>>(gen::arbitrary<PT>()),
        [](const array<PT, 2> pts) {
            PT a = min(pts[0], pts[1]);
            PT b = max(pts[0], pts[1]);
            a.coords[2] = b[2];
          return Shape(a, b);
        });
    }
  };

}