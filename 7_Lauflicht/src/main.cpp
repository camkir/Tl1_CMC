#include <Arduino.h>

int led[] = {

};
int num_leds = sizeof(led)/sizeof(led[0]);

struct ButtonState {
  int current;
  int previous;
};

ButtonState button1State = {0,0};
ButtonState button2State = {0,0};
ButtonState button3State = {0,0};

const int poti = A0;    //change speed
const int button1 = A1; //mode1: single LED
const int button2 = A2; //mode2: knightrider
const int button3 = A3; //mode3

void initLED(){
  for(int i=0; i<num_leds; i++){
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], HIGH);
  }
}

void pulseLED(){

}

void setup(){
  init();
  initLED();

  pinMode(poti, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
}

void loop(){
  button1State.current = analogRead(button1);
  button2State.current = analogRead(button2);
  button3State.current = analogRead(button3);

  if(button1State.current > button1State.previous){
    //single LED pulsing
    pulseLED();
  }

  if (button2State.current > button2State.previous){
    //knightrider

  }

  if (button3State.current > button3State.previous){
    //unknown
  }

  button1State.previous = button1State.current;
  button2State.previous = button2State.current;
  button3State.previous = button3State.current;
}