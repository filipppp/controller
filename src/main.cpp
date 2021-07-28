#include <NeoSWSerial.h>
#include "../.pio/libdeps/uno/PacketSerial/src/PacketSerial.h"

const int PIN_AN_THROTTLE = 0;
const int PIN_X_JOY = 2;
const int PIN_Y_JOY = 1;

// https://forum.arduino.cc/index.php?action=dlattach;topic=596828.0;attach=294205
NeoSWSerial HC12neo(6, 5); // TX Pin HC12 => RX PIN 5 | TX PIN 6 => RX PIN HC12
PacketSerial HC12;

/* Sensor variable */
struct CONTROLS {
    int16_t throttle = 0;
    int16_t x_joy = 512;
    int16_t y_joy = 512;
} controls;

struct SENSORS {
    float temperature = 0;
    float altitude = 0;
    float x_angle = 0;
    float y_angle = 0;
    float z_angle = 0;
    float q[4] = {0,0,0,0};
} sensors;

uint32_t last_sensor_update = millis();
uint32_t last_control_broadcast = millis();

void update_control_data() {
    int pot_val = analogRead(PIN_AN_THROTTLE);
    controls.throttle = map(pot_val, 40, 1023, 0, 180);
    if (controls.throttle < 0) { controls.throttle = 0; }
    controls.x_joy = analogRead(PIN_X_JOY);
    controls.y_joy = analogRead(PIN_Y_JOY);
}

void print_all_data() {
    Serial.print(controls.throttle);
    Serial.print(";");
    Serial.print(controls.x_joy);
    Serial.print(";");
    Serial.print(controls.y_joy);
    Serial.print(";");
    Serial.print(sensors.altitude);
    Serial.print(";");
    Serial.print(sensors.temperature);
    Serial.print(";");
    Serial.print(sensors.x_angle);
    Serial.print(";");
    Serial.print(sensors.y_angle);
    Serial.print(";");
    Serial.print(sensors.z_angle);
    for (int i = 0; i < 4; ++i) {
        Serial.print(";");
        Serial.print(sensors.q[i]);
    }
    Serial.print(";");
    Serial.println(millis() - last_sensor_update > 2000 ? 1 : 0);
}

void onPacketReceived(const uint8_t* buffer, size_t size) {
    if (size == sizeof(sensors)) {
        /** Update controls **/
        memcpy(&sensors, buffer, size);
        last_sensor_update = millis();

        /** Send current telemetry back **/
        HC12.send((uint8_t *)&controls, sizeof(controls));
        last_control_broadcast = millis();
    }
}

void setup() {
    Serial.begin(9600);             // Serial port to computer
    HC12neo.begin(9600);               // Serial port to HC12
    HC12.setStream(&HC12neo);
    HC12.setPacketHandler(&onPacketReceived);
}

void loop() {
    update_control_data();
    HC12.update();

    if (millis() - last_control_broadcast > 500) {
        HC12.send((uint8_t *)&controls, sizeof(controls));
        last_control_broadcast = millis();
    }

    print_all_data();
}



