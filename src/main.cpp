#include <NeoSWSerial.h>

const int PIN_AN_THROTTLE = 0;

NeoSWSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin
void setup() {
    Serial.begin(9600);             // Serial port to computer
    HC12.begin(9600);               // Serial port to HC12
}
void loop() {
    int pot_val = analogRead(PIN_AN_THROTTLE);
    int val = map(pot_val, 40, 1023, 0, 180);
    if (val < 0) {val=0;}
    Serial.println(val);
    HC12.write(val);
}