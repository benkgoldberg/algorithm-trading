#pragma once

#include <atomic>

class AtomicDouble {
public:
    AtomicDouble(double value = 0.0) : m_value(value) {}

    void store(double value) {
        m_value.store(value);
    }

    double load() const {
        return m_value.load();
    }

    double fetch_add(double arg) {
        double old = m_value.load();
        while (!m_value.compare_exchange_weak(old, old + arg));
        return old;
    }

    double fetch_sub(double arg) {
        double old = m_value.load();
        while (!m_value.compare_exchange_weak(old, old - arg));
        return old;
    }

    operator double() const {
        return load();
    }

private:
    std::atomic<double> m_value;
};