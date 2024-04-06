#include "noisy.h"
#include <cassert>
#include <utility>

vz::Noisy make_noisy() { return {}; }

void test() {
    vz::Noisy n0;
    vz::Noisy n1 = make_noisy();
    vz::Noisy n2 = n0;
    vz::Noisy n3 = std::move(n2);
    n0 = n1;
    n2 = std::move(n3);
}

int main() {
    test();
    assert(vz::Counters::instance().def_ctor_count () +
           vz::Counters::instance().copy_ctor_count() +
           vz::Counters::instance().move_ctor_count() == vz::Counters::instance().instance_count()); // invariant
    assert(vz::Counters::instance().instance_count() == vz::Counters::instance().dtor_count()); // leak
}
