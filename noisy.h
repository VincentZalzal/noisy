#pragma once

#include <iostream>

namespace vz {

class Counters {
public:
    static Counters& instance() {
        static Counters counters;
        return counters;
    }

    unsigned next_id() { return m_instance_count++; }

    void inc_def_ctor   () { ++m_def_ctor_count   ; }
    void inc_copy_ctor  () { ++m_copy_ctor_count  ; }
    void inc_move_ctor  () { ++m_move_ctor_count  ; }
    void inc_copy_assign() { ++m_copy_assign_count; }
    void inc_move_assign() { ++m_move_assign_count; }
    void inc_dtor       () { ++m_dtor_count       ; }

    void reset() {
        m_instance_count    = 0;
        m_def_ctor_count    = 0;
        m_copy_ctor_count   = 0;
        m_move_ctor_count   = 0;
        m_copy_assign_count = 0;
        m_move_assign_count = 0;
        m_dtor_count        = 0;
    }

    unsigned instance_count   () const { return m_instance_count   ; }
    unsigned def_ctor_count   () const { return m_def_ctor_count   ; }
    unsigned copy_ctor_count  () const { return m_copy_ctor_count  ; }
    unsigned move_ctor_count  () const { return m_move_ctor_count  ; }
    unsigned copy_assign_count() const { return m_copy_assign_count; }
    unsigned move_assign_count() const { return m_move_assign_count; }
    unsigned dtor_count       () const { return m_dtor_count       ; }

private:
    Counters() = default;
    ~Counters() {
        std::cout
            << "---------------------------------\n"
            << "Counters\n"
            << "  Instance count:            " << m_instance_count << '\n';
        if (m_def_ctor_count != 0)
            std::cout << "  Default constructor count: " << m_def_ctor_count << '\n';
        if (m_copy_ctor_count != 0)
            std::cout << "  Copy constructor count:    " << m_copy_ctor_count << '\n';
        if (m_move_ctor_count != 0)
            std::cout << "  Move constructor count:    " << m_move_ctor_count << '\n';
        if (m_copy_assign_count != 0)
            std::cout << "  Copy assignment count:     " << m_copy_assign_count << '\n';
        if (m_move_assign_count != 0)
            std::cout << "  Move assignment count:     " << m_move_assign_count << '\n';
        if (m_dtor_count != 0)
            std::cout << "  Destructor count:          " << m_dtor_count << '\n';
    }

    unsigned m_instance_count    = 0;
    unsigned m_def_ctor_count    = 0;
    unsigned m_copy_ctor_count   = 0;
    unsigned m_move_ctor_count   = 0;
    unsigned m_copy_assign_count = 0;
    unsigned m_move_assign_count = 0;
    unsigned m_dtor_count        = 0;
};

class Noisy {
public:
    Noisy() {
        std::cout << "Noisy(" << m_id << "): default constructor\n";
        Counters::instance().inc_def_ctor();
    }

    Noisy(const Noisy& other) {
        std::cout << "Noisy(" << m_id << "): copy constructor from Noisy(" << other.m_id << ")\n";
        Counters::instance().inc_copy_ctor();
    }

    Noisy(Noisy&& other) noexcept {
        std::cout << "Noisy(" << m_id << "): move constructor from Noisy(" << other.m_id << ")\n";
        Counters::instance().inc_move_ctor();
    }

    ~Noisy() {
        std::cout << "Noisy(" << m_id << "): destructor\n";
        Counters::instance().inc_dtor();
    }

    Noisy& operator=(const Noisy& other) {
        std::cout << "Noisy(" << m_id << "): copy assignment from Noisy(" << other.m_id << ")\n";
        Counters::instance().inc_copy_assign();
        return *this;
    }

    Noisy& operator=(Noisy&& other) noexcept {
        std::cout << "Noisy(" << m_id << "): move assignment from Noisy(" << other.m_id << ")\n";
        Counters::instance().inc_move_assign();
        return *this;
    }

    unsigned id() const { return m_id; }

private:
    unsigned m_id = Counters::instance().next_id();
};

}
