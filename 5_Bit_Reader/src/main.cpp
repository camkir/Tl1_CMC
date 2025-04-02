#include <Arduino.h>
#include <mfs.h>

struct ButtonState {
  int current;
  int previous;
};

ButtonState button1State = {0,0};
ButtonState button2State = {0,0};
ButtonState button3State = {0,0};

const int button1 = A1;
const int button2 = A2;
const int button3 = A3;

const int bits = 10;
int inputs[bits] = {0};
int index = 0;

int binary2dec(int* arr) {
  int sum = 0;
  for(int i = 0; i < bits; i++) {
      sum += arr[i] * (1 << i); //leftshift == 2^i
  }
  return sum;
}

void setup(){
  init();
  initMFS();

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
}

void loop(){
  button1State.current = digitalRead(button1);
  button2State.current = digitalRead(button2);
  button3State.current = digitalRead(button3);

  if(button1State.current < button1State.previous){
    inputs[index++] = 1;  //append 1
  }

  if(button2State.current < button2State.previous){
    inputs[index++] = 0;  //append 0
  }

  if(button3State.current < button3State.previous){
    index--;
    inputs[index] = 0;  //delete last entry & move back
  }

  int sum = binary2dec(inputs);
  writeDecToSegment(sum);

  button1State.previous = button1State.current;
  button2State.previous = button2State.current;
  button3State.previous = button3State.current;
}