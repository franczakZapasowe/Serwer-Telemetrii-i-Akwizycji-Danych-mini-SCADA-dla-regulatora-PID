#pragma once
#include <random>
class ProcessSimulator {
    float m_pv{};
    float m_alfa;
    float m_last_pv{};
    std::random_device rd;
    std::mt19937 m_rd;
    std::normal_distribution<float> m_dis;
public:
    ProcessSimulator(float alfa)
    :m_alfa(alfa),m_rd(rd()),m_dis(0.0f,0.1f){}

    [[nodiscard]] float getPV()const { return m_pv; }  // musi zostac uzyte

    void updatePv(float lastU) {
        m_pv = m_last_pv + m_alfa * (lastU - m_last_pv) + m_dis(m_rd);
        m_last_pv = m_pv;
    }
};

