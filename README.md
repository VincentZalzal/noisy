# noisy

`vz::Noisy` is a C++ type you can use to quickly check which special member functions (constructors, assignment operators, destructors) are being called by a block of code.

## Requirements

There is no dependency or build step. Just copy *Noisy.h* into your project, or use it directly on Compiler Explorer.

`vz::Noisy` works with C\+\+11 and up.

## Intended uses

This project is kept as simple as possible. It is not intended for production code.

### Code exploration

The main use-case is code exploration, especially on [Compiler Explorer](https://godbolt.org/).

Just use the `vz::Noisy` type and a message is printed for each call to a special member function. A summary is also printed at the end of the program. An operation must be called at least once to appear in the summary.

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
Noisy( 0): default constructor
Noisy( 0): destructor

===== Noisy counters =====
Default constructor count:  1
Destructor count:           1
```
(see it on [Compiler Explorer](https://godbolt.org/z/e586fqj7e))

However, if you change the main function slightly:
```cpp
int main() {
    vz::Noisy x = std::move(make_noisy());
}
```
you now get this:
```
Noisy( 0): default constructor
Noisy( 1): move constructor from Noisy( 0)
Noisy( 0): destructor
Noisy( 1): destructor

===== Noisy counters =====
Default constructor count:  1
Move constructor count:     1
Destructor count:           2
```
(see it on [Compiler Explorer](https://godbolt.org/z/MnE81PT7q))

### Testing

`vz::Noisy` relies on global counters. You can query and reset the counters for testing purposes.

Available functions:
- `vz::Noisy::counters()` returns a reference to the global instance of `vz::Counters`.
- `vz::Counters` is a copyable type that you can also construct, for example with the expected counter values.
- `vz::Counters` instances can be compared using `operator==` and printed using `operator<<`.
- `counters.reset()` resets the counters.
- `counters.leaks()` returns `true` if the number of constructor calls does not match the number of destructor calls.
- `vz::Noisy::set_verbose(false)` disables automatic printing.

For example, you can use `vz::Noisy` to test that a custom container does not leak or copy objects.
```cpp
struct MyVectorTest : ::testing::Test {
    MyVectorTest() {
        vz::Noisy::set_verbose(false);
        vz::Noisy::counters().reset();
    }
};

TEST_F(MyVectorTest, DoesNotLeak) {
    {
        MyVector<vz::Noisy> vec;
        vec.emplace_back();
    }
    EXPECT_FALSE(vz::Noisy::counters().leaks()) << vz::Noisy::counters();
}

TEST_F(MyVectorTest, DoesNotCopyWhenReallocating) {
    MyVector<vz::Noisy> vec;
    for (int i = 0; i < 100; i++)
        vec.emplace_back();
    EXPECT_EQ(vz::Noisy::counters().m_copy_ctor, 0);
    EXPECT_EQ(vz::Noisy::counters().m_copy_assign, 0);
}
```
(see it on [Compiler Explorer](https://godbolt.org/z/14MWj7944))

If, for example, the container does leak objects, you would get the following failure message:
```
Value of: vz::Noisy::counters().leaks()
  Actual: true
Expected: false
Default constructor count:  1
```

### Comparing algorithms

You can use `vz::Noisy` to compare the number of times special member functions are called for two algorithms.
```cpp
struct NoisyInt {
    int m_i;
    vz::Noisy m_noisy;

    friend bool operator<(const NoisyInt& lhs, const NoisyInt& rhs) {
        return lhs.m_i < rhs.m_i;
    }
};

std::vector<NoisyInt> make_random_vec(int size) {...}

int main() {
    vz::Noisy::set_verbose(false);

    auto vec1 = make_random_vec(128);
    auto vec2 = vec1;

    vz::Noisy::counters().reset();
    std::sort(vec1.begin(), vec1.end());
    std::cout << "==== sort ====\n" << vz::Noisy::counters();

    vz::Noisy::counters().reset();
    std::stable_sort(vec2.begin(), vec2.end());
    std::cout << "\n==== stable_sort ====\n" << vz::Noisy::counters();
}
```
which outputs:
```
==== sort ====
Move constructor count:    244
Move assignment count:     666
Destructor count:          244

==== stable_sort ====
Move constructor count:    172
Move assignment count:     1001
Destructor count:          172
```
(see it on [Compiler Explorer](https://godbolt.org/z/cbrKjYM7a))

## Known limitations

`vz::Noisy` is not thread-safe, as it uses global counters and `std::cout`.
