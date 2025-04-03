#include <Arduino.h>
#include <mfs.h>

int led[] = {
  13, //LED1
  12, //LED2
  11, //LED3
  10, //LED4
};

int potti = A0;
//int msg_bits = 10;
//int disp_bits = 4;
//int binaryarray[msg_bits] = {0};

/*
void dec2bin(int decimal, int bits)
	int index = msg_bits - 1;
  while(index >= 0) {
      int dual = 1 << index;
      if(decimal >= dual) {
          decimal = decimal - dual;
          binaryarray[index] = 1;
      } else {
          binaryarray[index] = 0;
      }
      index--;
  }
}
*/

int* dec2bin(int decimal, int bits) {

  //this is c magic which is beyond my comprehension
  //int binaryarray[bits] = {0} --> why no work?
  static int binaryarray[32];
  for(int i = 0; i < bits; i++) {
      binaryarray[i] = 0;
  }
  
  //decimal to binary, LSB = arr[0], MSB = arr[bits-1]
  int index = bits - 1;
  while(index >= 0) {
      int dual = 1 << index;
      if(decimal >= dual) {
          decimal = decimal - dual;
          binaryarray[index] = 1;
      } else {
          binaryarray[index] = 0;
      }
      index--;
  }
  
  return binaryarray;
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
  int max_length = 10;
  int decimal = analogRead(potti);

  int* arr = dec2bin(decimal, max_length);

  //write last 4 bits to LEDs and 7segment
  for(int i=0; i<4; i++){
    digitalWrite(led[i], !arr[i]);
    writeCharToSegment(i, arr[i], 0);
  }
}
