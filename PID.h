#pragma once
class PID {
    float m_Kp, m_Ki, m_Kd;
    float m_error{};              //uchyb
    float m_last_error {};
    float m_P{}, m_I{}, m_D{};    // P I D
    float m_output{};             // wyjscie
    float m_last_I{};
    float m_antiWindupMin, m_antiWindupMax, m_outputMin, m_outputMax;
    public:
    PID (float kp, float ki, float kd, float antiWindupMin, float antiWindupMax, float outputMin, float outputMax)
        : m_Kp(kp), m_Ki(ki), m_Kd(kd),
        m_antiWindupMin(antiWindupMin),m_antiWindupMax(antiWindupMax),
        m_outputMin(outputMin), m_outputMax(outputMax){}

    void update(float sp, float pv, float dt );
    [[nodiscard]] float getOutput() const {return m_output;}
    [[nodiscard]] float getError()const {return m_error;}
};

