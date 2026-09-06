#pragma once

class PID {
    float m_Kp, m_Ki, m_Kd;
    float m_error{};              //uchyb
    float m_last_error {};
    float m_P{}, m_I{}, m_D{};    // P I D
    float m_dt;                   // delta t
    float m_output{};             // wyjscie
    float m_last_I{};

    public:
    PID (float kp, float ki, float kd, float dt)
        : m_Kp(kp), m_Ki(ki), m_Kd(kd), m_dt(dt) {}

    float update(float sp, float pv );

};

