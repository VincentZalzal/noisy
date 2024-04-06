# noisy

`vz::Noisy` is a type you can use to quickly check which special member functions (constructors, assignment operators, destructors) are being called by a block of code.

## Requirements

There is no dependency or build step. Just copy *Noisy.h* into your project. The *CMakeLists.txt* in this repository is there only to compile the *test.cpp* example and it is not necessary to use `vz::Noisy`.

Even if the example requires C\+\+20, *Noisy.h* itself requires C\+\+11 only.

## Intended use

This project is kept as simple as possible. It is not intended for production code.

The main use-case is code exploration, especially on [Compiler Explorer](https://godbolt.org/).

*Compiler Explorer* allows including a file directly using a URL. Thus, you could for example confirm that [guaranteed copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) works as you expect:
```cpp
#include <https://raw.githubusercontent.com/VincentZalzal/noisy/main/noisy.h>

vz::Noisy make_noisy() { return {}; }

int main() {
    vz::Noisy x = vz::Noisy(make_noisy());
}
```
which outputs:
```
Noisy(0): default constructor
Noisy(0): destructor
---------------------------------
Counters
  Instance count:            1
  Default constructor count: 1
  Destructor count:          1
```
(see it on [Compiler Explorer](https://godbolt.org/z/fPEzc4do4))

You can also use it inside your tests. For example, after testing your generic function with `vz::Noisy`, you can validate the number of calls for all special member functions:
```cpp
    const vz::Counters expected_counts = {
        .m_instance    = 5,
        .m_def_ctor    = 3,
        .m_copy_ctor   = 1,
        .m_move_ctor   = 1,
        .m_copy_assign = 1,
        .m_move_assign = 1,
        .m_dtor        = 5,
    };
    assert(vz::Noisy::counters() == expected_counts);
```

You can see an example in [test.cpp](test.cpp) (results on [Compiler Explorer](https://godbolt.org/z/1nc33dYqG)).

## Known limitations

`vz::Noisy` is not thread-safe, as it is using global counters and `std::cout`.
