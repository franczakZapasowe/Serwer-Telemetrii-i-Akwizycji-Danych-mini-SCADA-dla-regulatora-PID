#include "PID.h"
#include <algorithm>
// ugly code ;(
void  PID::update(float sp, float pv) {
    m_error = sp - pv;
    m_P = m_Kp * m_error;
    m_I = m_last_I + m_Ki * m_error * m_dt;
    m_I = std::clamp(m_I, m_antiWindupMin, m_antiWindupMax);
    m_D = m_Kd * ( (m_error - m_last_error)/m_dt);
    m_last_I = m_I;
    m_last_error = m_error;
    m_output = m_P + m_I + m_D;
   m_output = std::clamp(m_output, m_outputMin, m_outputMax);
}

