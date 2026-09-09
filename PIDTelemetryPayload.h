#pragma once
#include <stdint.h>

#pragma pack(push, 1)
struct PIDTelemetryPayload {
    uint32_t packet_id;        // Monotonicznie rosnący identyfikator, do detekcji gubienia pakietów
    uint64_t timestamp_ms;     // Czas wygenerowania próbki na węźle (np. epoch w milisekundach)
    float setpoint;            // Wartość zadana (SP)
    float process_variable;    // Wartość mierzona (PV) - zaszumiona
    float control_output;      // Wartość sterująca (CV / U)
    float error;               // Aktualny uchyb (e)
    uint16_t status_flags = 1;     // Flagi bitowe (np. bit 0: ALARM, bit 1: MANUAL_MODE)
    uint16_t checksum;         // Suma kontrolna (np. CRC16) upewniająca nas o integralności payloadu
};
#pragma pack(pop)