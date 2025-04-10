#include <Arduino.h>
#include <mfs.h>

int led[] = {
  13, //LED1
  12, //LED2
  11, //LED3
  10, //LED4
};

const int potti = A0;
const int msg_bits = 10;
const int disp_bits = 4;
int binaryarray[msg_bits] = {0};

void dec2bin(int decimal) {
  for(int index = msg_bits - 1; index >= 0; index--) {
      int dual = 1 << index;
      if(decimal >= dual) {
          decimal -= dual;
          binaryarray[index] = 1;
      } else {
          binaryarray[index] = 0;
      }
  }
}

void setup(){
  init();
  initMFS();

  //set all LEDs to OUTPUT
  int num_led = sizeof(led)/sizeof(led[0]);
  for(int i=0; i<num_led; i++){
      pinMode(led[i], OUTPUT);
      digitalWrite(led[i], HIGH);
  }

  pinMode(potti, INPUT);
}

void loop() {
  int decimal = analogRead(potti);

  dec2bin(decimal);

  //write last 4 bits to LEDs and 7segment
  for(int i=0; i<4; i++){
    digitalWrite(led[i], !binaryarray[i]);
    writeCharToSegment(i, binaryarray[i], 0);
  }
}
