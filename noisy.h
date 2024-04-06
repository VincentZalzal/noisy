#pragma once

#include <iostream>

namespace vz {

struct Counters {
    unsigned m_instance    = 0;
    unsigned m_def_ctor    = 0;
    unsigned m_copy_ctor   = 0;
    unsigned m_move_ctor   = 0;
    unsigned m_copy_assign = 0;
    unsigned m_move_assign = 0;
    unsigned m_dtor        = 0;

    void reset() {
        *this = {};
    }

    void print() const {
        std::cout
            << "---------------------------------\n"
            << "Counters\n"
            << "  Instance count:            " << m_instance << '\n';
        if (m_def_ctor != 0)
            std::cout << "  Default constructor count: " << m_def_ctor << '\n';
        if (m_copy_ctor != 0)
            std::cout << "  Copy constructor count:    " << m_copy_ctor << '\n';
        if (m_move_ctor != 0)
            std::cout << "  Move constructor count:    " << m_move_ctor << '\n';
        if (m_copy_assign != 0)
            std::cout << "  Copy assignment count:     " << m_copy_assign << '\n';
        if (m_move_assign != 0)
            std::cout << "  Move assignment count:     " << m_move_assign << '\n';
        if (m_dtor != 0)
            std::cout << "  Destructor count:          " << m_dtor << '\n';
    }

    friend bool operator==(const Counters& lhs, const Counters& rhs) {
        return
            lhs.m_instance    == rhs.m_instance    &&
            lhs.m_def_ctor    == rhs.m_def_ctor    &&
            lhs.m_copy_ctor   == rhs.m_copy_ctor   &&
            lhs.m_move_ctor   == rhs.m_move_ctor   &&
            lhs.m_copy_assign == rhs.m_copy_assign &&
            lhs.m_move_assign == rhs.m_move_assign &&
            lhs.m_dtor        == rhs.m_dtor        ;
    }

    friend bool operator!=(const Counters& lhs, const Counters& rhs) { return !(lhs == rhs); }
};

namespace detail {

struct NoisyCounters : Counters {
    ~NoisyCounters() {
        print();
    }

    void print_and_reset() {
        if (m_instance != 0) {
            print();
            reset();
        }
    }

    unsigned next_id() { return m_instance++; }
};

}

class Noisy {
private:
    static detail::NoisyCounters& noisy_counters() {
        static detail::NoisyCounters counts;
        return counts;
    }

public:
    static const Counters& counters() { return noisy_counters(); }
    static void reset_counters() { noisy_counters().print_and_reset(); }

    Noisy() {
        std::cout << "Noisy(" << m_id << "): default constructor\n";
        noisy_counters().m_def_ctor++;
    }

    Noisy(const Noisy& other) {
        std::cout << "Noisy(" << m_id << "): copy constructor from Noisy(" << other.m_id << ")\n";
        noisy_counters().m_copy_ctor++;
    }

    Noisy(Noisy&& other) noexcept {
        std::cout << "Noisy(" << m_id << "): move constructor from Noisy(" << other.m_id << ")\n";
        noisy_counters().m_move_ctor++;
    }

    ~Noisy() {
        std::cout << "Noisy(" << m_id << "): destructor\n";
        noisy_counters().m_dtor++;
    }

    Noisy& operator=(const Noisy& other) {
        std::cout << "Noisy(" << m_id << "): copy assignment from Noisy(" << other.m_id << ")\n";
        noisy_counters().m_copy_assign++;
        return *this;
    }

    Noisy& operator=(Noisy&& other) noexcept {
        std::cout << "Noisy(" << m_id << "): move assignment from Noisy(" << other.m_id << ")\n";
        noisy_counters().m_move_assign++;
        return *this;
    }

    unsigned id() const { return m_id; }

private:
    unsigned m_id = noisy_counters().next_id();
};

}
