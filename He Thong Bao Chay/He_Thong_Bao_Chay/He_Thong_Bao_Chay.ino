#include <Wire.h>
int speaker = 4;                 // loa,den ket noi Pin 4
int gas = 2;                    // out ket noi Pin 2
int led = 3;// led he thong bao chay 

void setup() {
  // khai bao cac pin cho he thong bao chay 
  pinMode(gas, INPUT);
  pinMode(speaker, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  // ket thuc khai bao he thong bao chay
}
void loop() {
  // vong lap cho he thong bao chay
  if (digitalRead(gas) == HIGH)
  {
   for(int i = 0 ; i< 30; i++){
    digitalWrite(led, HIGH);
    digitalWrite(speaker, HIGH);
    delay(65
    );
    digitalWrite(led, LOW);
    digitalWrite(speaker, LOW);
    delay(100);
    
   }
  }
  else
  {
    digitalWrite(led, LOW);
    digitalWrite(speaker, LOW);
  }
  // ket thuc vong lap he thong bao chay
}
