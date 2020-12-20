#include <Servo.h>     
Servo myservo; 

int pos = 0;
 
void setup() { 
    myservo.attach(8);
} 
 
void loop() { 
   for(pos = 180; pos>=1; pos-=1) {                           
        myservo.write(pos);
        delay(15);
    } 
    
  
}
