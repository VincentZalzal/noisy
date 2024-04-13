#include <cassert>
#include <utility>

#include "noisy.h"

using vz::Noisy;

Noisy make_noisy() { return {}; }

void test() {
    Noisy n0;                 // default ctor
    Noisy n1 = make_noisy();  // default ctor (guaranteed copy elision in C++17)
    Noisy n2 = n0;            // copy ctor (no assignment)
    Noisy n3 = std::move(n2); // move ctor (no assignment)
    n0 = n1;                  // copy assignment
    n2 = make_noisy();        // default ctor + move assignment
}

int main() {
    test();
    const auto counts = Noisy::counters();
    
    assert(counts.m_def_ctor + counts.m_copy_ctor + counts.m_move_ctor == counts.m_dtor); // no leak in user code

    // specific to test()
    const vz::Counters expected_counts = {
        .m_def_ctor    = 3,
        .m_copy_ctor   = 1,
        .m_move_ctor   = 1,
        .m_copy_assign = 1,
        .m_move_assign = 1,
        .m_dtor        = 5,
    };
    assert(counts == expected_counts);
}
