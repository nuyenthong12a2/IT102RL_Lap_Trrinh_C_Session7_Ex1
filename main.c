#include <stdio.h>

struct SmartDevice {
    int id;
    float voltage;
    float current;
    float power_watt;
    int is_active;
    int is_overloaded;
};

int main() {
    struct SmartDevice heater;
    heater.id = 101;
    heater.voltage = 220.0f;
    heater.current = 35.0f;
    heater.is_active = 1;
    heater.is_overloaded = 0;
    heater.power_watt = 0.0f;

    if (heater.is_active == 1 && heater.current > 30.0f) {
        heater.is_overloaded = 1;
        heater.is_active = 0;
    }

    if (heater.is_active == 1) {
        heater.power_watt = heater.voltage * heater.current;
    } else {
        heater.power_watt = 0.0f;
    }

    printf("=== HETHONG GIAM SAT THIET BI SMART HOME ===\n");
    printf("Ma thiet bi: %d\n", heater.id);
    printf("Trang thai hoat dong: %s\n", heater.is_active ? "BAT" : "TAT");
    printf("Cong suat tieu thu: %.2f W\n", heater.power_watt);
    printf("Canh bao qua tai (>30A): %s\n", heater.is_overloaded ? "CO (DA NGAT DIEN)" : "KHONG");

    return 0;
}