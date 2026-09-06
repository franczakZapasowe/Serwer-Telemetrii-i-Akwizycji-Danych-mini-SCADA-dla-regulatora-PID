#pragma once
#include <algorithm>
#include <random>
class PV {
    float m_pv{};
    float m_alfa;
    float m_last_pv{};
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::normal_distribution<float> m_dis;

public:
    PV(float alfa)
    :m_alfa(alfa),m_gen(m_rd()),m_dis(0.0f, 1.0f) {}

    [[nodiscard]] float getPV()const { return m_pv; }

    float szum = m_dis(m_gen);
    void updatePv(float lastU) {
        m_pv = m_last_pv + m_alfa * (lastU - m_last_pv) + szum;
        m_last_pv = m_pv;
    }
};

