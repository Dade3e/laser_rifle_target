// ATMEL ATTINY 25/45/85 / ARDUINO
//
//                  +-\/-+
// Ain0 (D 5) PB5  1|    |8  Vcc
// Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1
// Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
//            GND  4|    |5  PB0 (D 0) pwm0   -> speaker
//                  +----+


#include "avr_sound.h"
#include "Quack_Sound_Effect.h"

#define AnalogPin 3

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
  pinMode(AnalogPin,INPUT);
  pinMode(0,OUTPUT);
  if(info || debug)
    Serial.begin(115200);

  avr_sound_init();

  //first setup photoresistor
  for(int i = 0; i< buffer_size; i++){
    int val = analogRead(AnalogPin);
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
  int val = analogRead(AnalogPin);
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
      delay(200);
      //for (int i=0; i<5; i++){
      digitalWrite(0, HIGH);
      delay(50);
      digitalWrite(0, LOW);
      //delay(30);
      //}

      //play_Quack_Sound_Effect();
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
