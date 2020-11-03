#include <NeoSWSerial.h>
#include <HC12.h>

const int PIN_AN_THROTTLE = 0;
const int PIN_X_JOY = 1;
const int PIN_Y_JOY = 2;

NeoSWSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

uint8_t throttle = 0;
uint8_t x_joy = 512;
uint8_t y_joy = 512;
uint8_t* payload;

void update_readings() {
    int pot_val = analogRead(PIN_AN_THROTTLE);
    throttle = map(pot_val, 40, 1023, 0, 180);
    x_joy = analogRead(PIN_X_JOY);
    y_joy = analogRead(PIN_Y_JOY);
}

void update_payload() {
    update_readings();
    payload[0] = throttle;
    payload[1] = x_joy;
    payload[2] = y_joy;
}


void setup() {
    Serial.begin(9600);             // Serial port to computer
    HC12.begin(9600);               // Serial port to HC12
}

void loop() {
    update_payload();
    if (throttle < 0) { throttle=0;}

    HC12.write(payload, 3);
}



