#include <Arduino.h>
#include <mfs.h>
#include <string.h>

const int msg_bits = 10;
const int display_bits = 4;

char* dec2binStr(uint16_t dec) {
    static char binary[msg_bits + 1];  // message length + terminator
    int i = 0;
    
    // Handle 0 case
    if (dec == 0) {
        binary[0] = '0';
        binary[1] = '\0';
        return binary;
    }
    
    // Convert to binary (lsb first)
    while (dec > 0) {
        binary[i++] = (dec & 1) ? '1' : '0';
        dec >>= 1;
    }
    binary[i] = '\0';
    return binary;
}

char* shortenBin(const char* bin) {
    static char shortenedBin[display_bits + 1];  // display message len + terminator
    
    // copy characters
    for (size_t i = 0; i < display_bits; i++) {
        shortenedBin[i] = bin[i];
    }
    shortenedBin[display_bits + 1] = '\0';
    return shortenedBin;
}
void setup(){
    Serial.begin(9600);

}

void loop(){
    uint16_t number = 1024;

    char* shortbin = shortenBin(dec2binStr(number));

    Serial.println(shortbin);
    delay(1000);
}