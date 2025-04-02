#include <Arduino.h>
#include <mfs.h>

const int button1 = A1;     // START / STOP / CONTINUE
const int button2 = A2;     // SHOW LAST LAP
const int button3 = A3;     // RESET TO 0

struct ButtonState {
    int current;
    int previous;
};

struct TimerState {
    unsigned long startTime;
    unsigned long stopTime;
    unsigned long currentTime;
    unsigned long accumulatedTime;
    unsigned long lastLapTime;
    bool isRunning;
};

ButtonState button1State = {0, 0};
ButtonState button2State = {0, 0};
ButtonState button3State = {0, 0};

TimerState timerState = {
    .startTime = 0,
    .stopTime = 0,
    .currentTime = 0,
    .accumulatedTime = 0,
    .lastLapTime = 0,
    .isRunning = false
};

void handleButton1();
void handleButton2();
void handleButton3();

void setup() {
    init();
    initMFS();

    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
    pinMode(button3, INPUT);
    button1State.previous = digitalRead(button1);
    button2State.previous = digitalRead(button2);
    button3State.previous = digitalRead(button3);
}

void loop() {
    // read current button states
    button1State.current = digitalRead(button1);
    button2State.current = digitalRead(button2);
    button3State.current = digitalRead(button3);

    // trigger on falling edge
    if (button1State.current < button1State.previous) {
        handleButton1();
    }

    if (button3State.current < button3State.previous) {
        handleButton3();
    }

    button1State.previous = button1State.current;
    button2State.previous = button2State.current;
    button3State.previous = button3State.current;

    if (timerState.isRunning) {
        unsigned long currentTime = millis() - timerState.startTime + timerState.accumulatedTime;
        writeDecToSegment(currentTime);
    } else if (button2State.current == LOW && timerState.stopTime != 0) {
        // handleButton2() --> write last lap to display while button2 held
        writeDecToSegment(timerState.lastLapTime);
    } else if (!timerState.isRunning && timerState.stopTime != 0) {
        writeDecToSegment(timerState.currentTime);
    }
}

void handleButton1() {
    if (!timerState.isRunning) {
        // starting from stopped state
        if (timerState.stopTime != 0) {
            // continue from previous time
            timerState.accumulatedTime = timerState.currentTime;
        }
        timerState.startTime = millis();
        timerState.isRunning = true;
    } else {
        //stop timer at new time
        timerState.stopTime = millis();
        timerState.currentTime = timerState.accumulatedTime + (timerState.stopTime - timerState.startTime);
        timerState.isRunning = false;
        // save last lap
        timerState.lastLapTime = timerState.currentTime - timerState.accumulatedTime;
    }
}

void handleButton2() {
    //Button2 is handled in loop bc of continuous display, but not making a function for this annoys me
    //therefore this stays (for emotional support)
}

void handleButton3() {
    //reset everything
    timerState.isRunning = false;
    timerState.startTime = 0;
    timerState.stopTime = 0;
    timerState.currentTime = 0;
    timerState.accumulatedTime = 0;
    timerState.lastLapTime = 0;
    writeDecToSegment(0);
}