#include <Arduino.h>

const int led[] = {
    13, //1
    12, //2
    11, //3
    10, //4
    5,  //5
    6   //6
};
int num_led = 0;

const int potti_pin = A0;
int potti_val = 0;

int BLINK_INTERVAL = 500;

void setup(){
    pinMode(potti_pin, INPUT);

    //set all LEDs to OUTPUT
    num_led = sizeof(led)/sizeof(led[0]);
    for(int i=0; i<num_led; i++){
        pinMode(led[i], OUTPUT);
        digitalWrite(led[i], HIGH);
    }
};

void loop(){
    potti_val = analogRead(potti_pin);

    //map potti values to LEDs
    int numLEDs = map(potti_val, 0, 1023, 0, num_led);
    
    //ON
    for (int i = 0; i < numLEDs; i++) {
        digitalWrite(led[i], LOW);
    }
    
    //STAY ON
    delay(BLINK_INTERVAL / 2);
      
    //OFF
    for (int i = 0; i < num_led; i++) {
        digitalWrite(led[i], HIGH);
    }

    //STAY OFF  
    delay(BLINK_INTERVAL / 2);
}