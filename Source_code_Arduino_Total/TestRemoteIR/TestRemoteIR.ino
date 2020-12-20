#include <IRremote.h> 
int pos = 0;
const int pin = 2;
IRrecv irrecv(pin);
decode_results results;
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(3,OUTPUT);
}
int tam = 0;
void loop() {
 if(irrecv.decode(&results)){
  Serial.println(results.value,DEC);
  irrecv.resume();
 }
 if(results.value == 16738455){
  tam ++;
   if(tam ==1){
     digitalWrite(3,HIGH);
   }else if(tam ==2){
     digitalWrite(3,LOW);
     tam =0;
   }
 }

}
