#include "PID.h"
#include <algorithm>
// ugly code ;(
float PID::update(float sp, float pv) {
    m_error = sp - pv;
    m_P = m_Kp * m_error;
    m_I = m_last_I + m_Ki * m_error * m_dt;
    m_I = std::clamp(m_I, -1.0f, 1.0f);     //NARAZIE TAKI ZAKRES -1 DO 1 ALE NWM JAK KAZESZ DAC INNY TO DAMY INNY
    m_D = m_Kd * ( (m_error - m_last_error)/m_dt);
    m_last_I = m_I;
    m_last_error = m_error;
    m_output = m_P + m_I + m_D;
    return m_output = std::clamp(m_output, -100.0f, 100.0f);       //Tak samo jak anitwindup nie wiem jakie wartosci tu dac najwzyej zmienie
}

