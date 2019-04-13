#include "pitch.h"

//Define pins
int ldr[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
int laser[8] = {2, 3, 4, 6, 5, 7, 8, 9};
int speakerPin = 11;

//note matrix
int note[50] = {NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1,
                NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2,
                NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F1, NOTE_G3, NOTE_A3, NOTE_B3,
                NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,
                NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5,
                NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6,
                NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7,
                NOTE_C8};
int midiNote[8] = {24, 26, 28, 29, 31, 33, 35, 36};

int ref = 100; //referent sensor value
int octave = 5;
int output = 0;
int mode = 0;
int modeRefTime = 4000; //time to triger mode change
unsigned long laserCutTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void play(int notePos, int oct = octave){
  if(!output){
    tone(speakerPin, note[oct*7 + notePos], 100);
  }else{
    Serial.write(0x90);
    Serial.write(midiNote[notePos] + oct*12);
    Serial.write(0x7F);
  }
  delay(2);
}

void stopPlaying(int notePos, int oct = octave){
  noTone(speakerPin);
  Serial.write(0x90);
  Serial.write(midiNote[notePos] + oct*12);
  Serial.write(0x00);
}

void playMode(){
  for (int i = 0; i < 8; i++){
    digitalWrite(laser[i], HIGH);
  }
  for(int i = 0; i < 8; i++){
    if(analogRead(ldr[i]) < ref){
      play(i, octave);
    }else{
      stopPlaying(i);
    }
  }
}

void checkModeChange(int cmode){
  if(analogRead(ldr[cmode]) < ref && !laserCutTime[cmode]){
    for(int i = 0; i < 8; i++){
      laserCutTime[i] = 0;
    }

    laserCutTime[cmode] = millis();
  }

  if(laserCutTime[cmode] && millis() - laserCutTime[cmode] > modeRefTime){
    laserCutTime[cmode] = 0;
    play(cmode); //sound signal
    //turn off all other lasers
    for (int i = 0; i < cmode; i++){
    digitalWrite(laser[i], LOW);
    }

    for (int i = cmode + 1; i < 8; i++){
    digitalWrite(laser[i], LOW);
    }
    //wait until hand is moved
    while(analogRead(ldr[cmode]) < ref){}
    delay(20);
    //stop playing all notes
    for(int i = 0; i < 8; i++){
      stopPlaying(i);
    }
    //change mode
    mode = cmode;
  }

  if(analogRead(ldr[cmode]) > ref){
    laserCutTime[cmode] = 0;
  }
}

void changeOctave(){
  for (int i = 0; i < 8; i++){
    digitalWrite(laser[i], HIGH);
  }
  delay(20);
  for(int i = 0; i < 7; i++){
    if(analogRead(ldr[i]) < ref){
      play(i, i); //sound signal
      //turn off all other lasers
      for (int j = 0; j < i; j++){
      digitalWrite(laser[j], LOW);
      }
      for (int j = i + 1; j < 8; j++){
      digitalWrite(laser[j], LOW);
      }
      //wait until hand is moved
      while(analogRead(ldr[i]) < ref){}
      delay(10);
      octave = i;
      mode = 0;
      break;
    }
    delay(20);
  }
}

void changeOutput(){
  digitalWrite(laser[0], HIGH);
  digitalWrite(laser[1], HIGH);
  for(int i = 0; i < 2; i++){
    if(analogRead(ldr[i] < ref)){
      play(i, octave);
      digitalWrite(laser[(i + 1) % 2], LOW);
      while(analogRead(ldr[i] < ref)){}
      output = i;
      mode = 0;
    }
    delay(10);
  }
}

void setup() {
  for(int i = 0; i < 8; i++){
    pinMode(ldr[i], INPUT);
    pinMode(laser[i], OUTPUT);
  }
  pinMode(speakerPin, OUTPUT);
  //Serial.begin(9600);
}

void loop() {
  if(!mode){
    for(int cmode = 1; cmode < 4; cmode++){
      int cmode2 = mode;
      checkModeChange(cmode);
      if(mode != cmode2){
        break;
      }
    }
  }

  switch(mode){
    case 0:
      playMode();
      break;
    case 1:
      changeOctave();
      break;
    case 2:
      changeOutput();
  }
}
