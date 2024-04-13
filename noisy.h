#pragma once

#include <iomanip>
#include <iostream>

namespace vz {

struct Counters {
    unsigned m_def_ctor    = 0;
    unsigned m_copy_ctor   = 0;
    unsigned m_move_ctor   = 0;
    unsigned m_copy_assign = 0;
    unsigned m_move_assign = 0;
    unsigned m_dtor        = 0;

    void reset() {
        *this = {};
    }

    void print(const char* msg = "Counters") const {
        std::cout << "[ *** " << msg << " *** ]\n";
        print_counter("Default constructor count: ", m_def_ctor   );
        print_counter("Copy constructor count:    ", m_copy_ctor  );
        print_counter("Move constructor count:    ", m_move_ctor  );
        print_counter("Copy assignment count:     ", m_copy_assign);
        print_counter("Move assignment count:     ", m_move_assign);
        print_counter("Destructor count:          ", m_dtor       );
    }

    friend bool operator==(const Counters& lhs, const Counters& rhs) {
        return
            lhs.m_def_ctor    == rhs.m_def_ctor    &&
            lhs.m_copy_ctor   == rhs.m_copy_ctor   &&
            lhs.m_move_ctor   == rhs.m_move_ctor   &&
            lhs.m_copy_assign == rhs.m_copy_assign &&
            lhs.m_move_assign == rhs.m_move_assign &&
            lhs.m_dtor        == rhs.m_dtor        ;
    }

    friend bool operator!=(const Counters& lhs, const Counters& rhs) { return !(lhs == rhs); }

private:
    static void print_counter(const char* msg, unsigned value) {
        if (value != 0)
            std::cout << msg << std::setw(2) << value << '\n';
    }
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

    unsigned m_instance = 0;
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
        std::cout << *this << ": default constructor\n";
        noisy_counters().m_def_ctor++;
    }

    Noisy(const Noisy& other) {
        std::cout << *this << ": copy constructor from " << other << '\n';
        noisy_counters().m_copy_ctor++;
    }

    Noisy(Noisy&& other) noexcept {
        std::cout << *this << ": move constructor from " << other << '\n';
        noisy_counters().m_move_ctor++;
    }

    ~Noisy() {
        std::cout << *this << ": destructor\n";
        noisy_counters().m_dtor++;
    }

    Noisy& operator=(const Noisy& other) {
        std::cout << *this << ": copy assignment from " << other << '\n';
        noisy_counters().m_copy_assign++;
        return *this;
    }

    Noisy& operator=(Noisy&& other) noexcept {
        std::cout << *this << ": move assignment from " << other << '\n';
        noisy_counters().m_move_assign++;
        return *this;
    }

    unsigned id() const { return m_id; }

    friend std::ostream& operator<<(std::ostream& os, const Noisy& noisy) { return os << "Noisy(" << std::setw(2) << noisy.m_id << ')'; }

private:
    unsigned m_id = noisy_counters().next_id();
};

}
