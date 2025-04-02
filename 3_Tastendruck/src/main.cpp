#include <Arduino.h>
#include <mfs.h>

struct ButtonState {
    bool isPressed;
    long pressStartTime;
    bool wasJustPressed;
};

ButtonState buttonState = {false, 0, false};
int button = A1;
int potti = A0;
int short_count = 0;
int long_count = 0;
float min_length_ms = 1000;

void setup() {
    init();
    initMFS();
    pinMode(button, INPUT_PULLUP);  // Using internal pullup
    pinMode(potti, INPUT);
}

int displayInt(int short_press, int long_press) {
  return long_press * 100 + short_press;
}

void checkButtonPress() {
    int currentState = digitalRead(button);
    
    // PRESS
    if (!buttonState.isPressed && currentState == LOW) {  // LOW means pressed due to pullup
        buttonState.isPressed = true;
        buttonState.pressStartTime = millis();
        buttonState.wasJustPressed = true;
    }
    
    // RELEASE
    if (buttonState.isPressed && currentState == HIGH) {
        buttonState.isPressed = false;
        
        // increment values IF button was pressed
        if (buttonState.wasJustPressed) {
            unsigned long pressDuration = millis() - buttonState.pressStartTime;
            
            if (pressDuration >= min_length_ms) {
                long_count++;
            } else {
                short_count++;
            }
            
            buttonState.wasJustPressed = false;  // Reset flag
        }
    }
}

void loop() {
    checkButtonPress();
    
    if (short_count < 100 && long_count < 100) {
        int display_num = displayInt(short_count, long_count);
        writeDecToSegment(display_num);
    } else {
        writeHexToSegment(0xFFFF);
    }
}