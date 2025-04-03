#include <Arduino.h>
#include <mfs.h>

struct ButtonState  {
    int previous;
    int current;
};

ButtonState button1State = {0,0};
ButtonState button2State = {0,0};
ButtonState button3State = {0,0};

const int led[] = {
    13,         //CH1
    12,         //CH2
    11,
    10,
    5,
    6           //Sample & Hold
};
int num_leds = sizeof(led)/sizeof(led[0]);


const int channel[] = {
    A0, //poti
    A4,  //analog pin
};
int num_channels = sizeof(channel)/sizeof(channel[0]);

const int button1 = A1;     //Sample & Hold
const int button2 = A2;    
const int button3 = A3;     //Switch channel


int current_ch = 0;
bool samplehold_active = false;
int measurement_sum = 0;

int measureVoltage(int channel_num) {
    //measure voltage from ADC and map to 0-5V.
    int measurement_raw = analogRead(channel[channel_num]);
    return map(measurement_raw, 0, 1023, 0, 5000);
}

void displayChannel(int ch){
    //LED number is channel number
    for(int i=0; i<=num_channels-1; i++){
        digitalWrite(led[i], HIGH);
    }
    digitalWrite(led[ch], LOW);
}

void writeMeasurement(int meas){
    //handle 7 seg & decimal point
    writeDecToSegment(meas);
}

int measureMeanStatic(int channel, int sample_size){
    //collect n measurements and find their average --> outputs every 16 measurements
    long sum = 0;
    for(int i=0; i<sample_size; i++){
        sum += measureVoltage(channel);
        delay(1);   //prolly not necessary
    }
    return sum/sample_size;
}

int measureMeanCont(int channel, int sample_size){
    //continuously update the average by accepting a new value and removing the n-th value in the past
}

int calculateNoise(int measurement, int sample_size){
    //calculate noise of last x measurements
}

void sampleAndHold(){
}

void initLEDs(){
    for(int i=0; i<num_leds; i++){
        pinMode(led[i], OUTPUT);
        digitalWrite(led[i], HIGH);
    }
}


void setup(){
    init();
    initMFS();
    initLEDs();
    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
    pinMode(button3, INPUT);
    pinMode(channel[0], INPUT);
    pinMode(channel[1], INPUT);

    Serial.begin(9600);
}

void loop(){
    button1State.current = digitalRead(button1);
    button2State.current = digitalRead(button2);
    button3State.current = digitalRead(button3);

    int measurement = measureMeanStatic(current_ch, 16);

    if(button1State.current < button1State.previous){
        //toggle sample and hold mode
        samplehold_active = !samplehold_active;
        if(samplehold_active){
            sampleAndHold();
            digitalWrite(led[5], LOW);
        } else {digitalWrite(led[5], HIGH);}
    }

    if(button2State.current < button2State.previous){
        //do something cool and unique,  perhaps even interesting.
        //IDEAS: zeroing
    }

    if(button3State.current < button3State.previous){
        //switch channels
        if(current_ch == num_channels-1){
            current_ch = 0;
        } else {current_ch++;}
    }

    displayChannel(current_ch);
    writeMeasurement(measurement);

    button1State.previous = button1State.current;
    button2State.previous = button2State.current;
    button3State.previous = button3State.current;

    Serial.println(measurement);
}

