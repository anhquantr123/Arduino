
#include <Wire.h> 
#include <Servo.h> 
#define SERVO_PIN 13
int cambienanhsang = 12;
int cambienmua = 11;
Servo gServo;
void setup()
{
  pinMode(cambienanhsang,INPUT);
  pinMode(cambienmua,INPUT);
  gServo.attach(SERVO_PIN); 
 
}


void loop()
{
  int value = digitalRead(cambienmua);
  if(value == HIGH){
   gServo.write(0);
  int anhsang  = digitalRead(cambienanhsang);
  if(anhsang == 1){
     gServo.write(180);
  }else{
    gServo.write(0);
  }
  }else{                  
        gServo.write(180);
  }
 
}
