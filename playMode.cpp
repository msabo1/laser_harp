void playMode(){
  for(int i = 0; i<8; i++){
    if(analogRead(ldr[i]) < ref){
      play(note[octave*7 + i]);
    }
  }
}