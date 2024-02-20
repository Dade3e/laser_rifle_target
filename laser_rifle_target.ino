#include "avr_sound.h"
#include "Quack_Sound_Effect.h"

#define buffer_size 32
#define debug false
#define info false

int buffer[buffer_size];
int index = 0;
int offset = 20;
int media = 0;
int max = 0;
long j = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  if(info || debug)
    Serial.begin(115200);

  avr_sound_init();

  //first setup photoresistor
  for(int i = 0; i< buffer_size; i++){
    int val = analogRead(A0);
    if(val > max)
      max = val;
    buffer[i] = val;
  }
  max = 1000 - ((1000-max)/2);
  long sum = 0;
  for(int i = 0; i < buffer_size; i++){
    sum += buffer[i];
  }
  media = sum / buffer_size;
  
}

void loop() {
  int val = analogRead(A0);
  if(val > max)
    max = val;
  offset = (max - media) /2;
  if(val > media + offset){
    //qui fai qualcosa finche la madia arrai non raggiunge la soglia, circa buffer_size volte
    j+=1;
  }
  else{
    if(j > 0){
      //qui fai qualcosa 1 volta che il laser non colpisce piu il sensore, una volta sola
      if(info){
        Serial.print("HIT ");
        Serial.println(j);
      }
      /*
      for (int i=0; i<5; i++){
        digitalWrite(8, HIGH);
        delay(20);
        digitalWrite(8, LOW);
        delay(30);
      }
      */
      play_Quack_Sound_Effect();
    }
    j = 0;
  }
  buffer[index] = val;
  index += 1;
  //buffer[index] = val;
  //index += 1;
  if(index >= buffer_size) { index = 0; }
  long sum = 0;
  for(int i = 0; i < buffer_size; i++){
    sum += buffer[i];
  }
  media = sum / buffer_size;
  if(debug){
    Serial.print(val);
    Serial.print(" ");
    Serial.print(media);
    Serial.print(" ");
    Serial.print(max);
    Serial.print(" ");
    Serial.println(offset);
  }
  delay(1);
}
