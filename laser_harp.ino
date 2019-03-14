#include "pitch.h"

//Define pins
int ldr[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
int laser[8] = {2, 3, 4, 5, 6, 7, 8, 9};
int speakerPin = 10;

//note matrix
int note[50] = {NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1,
                NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2,
                NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F1, NOTE_G3, NOTE_A3, NOTE_B3,
                NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,
                NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5,
                NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6,
                NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7,
                NOTE_C8};

int ref = 800; //referent sensor value
int octave = 0;

void play(int note){
  tone(speakerPin, note, 100);
}

void playMode(){
  for (int i = 0; i<8; i++){
    digitalWrite(laser[i], HIGH);
  }

  for(int i = 0; i<8; i++){
    if(analogRead(ldr[i]) < ref){
      play(note[octave*7 + i]);
    }
  }
}

void setup() {
  for(int i = 0; i < 8; i++){
    pinMode(ldr[i], INPUT);
    pinMode(laser[i], OUTPUT);
  }
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  playMode();
}