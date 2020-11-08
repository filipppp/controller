#include <NeoSWSerial.h>

const int PIN_AN_THROTTLE = 0;
const int PIN_X_JOY = 2;
const int PIN_Y_JOY = 1;

NeoSWSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

int16_t throttle = 0;
int16_t x_joy = 512;
int16_t y_joy = 512;
unsigned char* payload = new unsigned char[6];

void update_readings() {
    int pot_val = analogRead(PIN_AN_THROTTLE);
    throttle = map(pot_val, 40, 1023, 0, 180);
    x_joy = analogRead(PIN_X_JOY);
    y_joy = analogRead(PIN_Y_JOY);
}

void update_payload() {
    update_readings();
    payload[0] = throttle >> 8;
    payload[1] = throttle & 0xFF;
    payload[2] = x_joy >> 8;
    payload[3] = x_joy & 0xFF;
    payload[4] = y_joy >> 8;
    payload[5] = y_joy & 0xF;
}

void printBits(unsigned int num)
{
    for(int bit=0;bit<(sizeof(unsigned int) * 8); bit++)
    {
        Serial.print(num & 0x01);
        num = num >> 1;
    }
}

void printChar(char a) {
    int i;
    for (i = 0; i < 8; i++) {
        Serial.print(!!((a << i) & 0x80));
    }
}

void setup() {
    Serial.begin(9600);             // Serial port to computer
    HC12.begin(9600);               // Serial port to HC12
}

void loop() {
    update_payload();
    if (throttle < 0) { throttle=0;}
    HC12.print('<');
    for (int i = 0; i < 6; i++) {
        HC12.print(payload[i]);
        if (i != 5) {
            HC12.write(";");
        }
    }
    Serial.println();
    HC12.print('>');

//    Serial.print("Throttle: ");
//    Serial.print(throttle);
//    Serial.print(" X_JOY: ");
//    Serial.print(x_joy);
//    Serial.print(" Y_JOY: ");
//    Serial.println(y_joy);
    delay(100);
}



