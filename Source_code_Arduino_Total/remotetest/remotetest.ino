/*
   -- New project --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.4.3 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.5.1 or later version;
     - for iOS 1.4.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "Smart Home"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,6,0,0,0,206,0,10,13,1,
  2,1,2,8,17,8,2,26,31,179,
  79,78,0,79,70,70,0,129,0,5,
  3,11,3,17,196,144,195,168,110,32,
  80,75,0,2,1,23,8,17,8,2,
  26,31,179,79,78,0,79,70,70,0,
  129,0,26,3,13,3,17,196,144,195,
  168,110,32,80,78,49,0,2,1,43,
  8,17,8,2,26,31,179,79,78,0,
  79,70,70,0,129,0,46,3,13,3,
  17,196,144,195,168,110,32,80,78,50,
  0,2,1,1,24,17,8,2,26,31,
  179,79,78,0,79,70,70,0,129,0,
  4,19,11,3,17,196,144,195,168,110,
  32,66,225,186,191,112,0,2,1,22,
  24,17,8,2,26,31,179,79,78,0,
  79,70,70,0,129,0,25,19,13,3,
  17,196,144,195,168,110,32,67,84,0,
  2,1,42,24,17,8,2,26,31,179,
  79,78,0,79,70,70,0,129,0,45,
  19,13,3,17,196,144,195,168,110,32,
  86,83,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  uint8_t switch_2; // =1 if switch ON and =0 if OFF 
  uint8_t switch_3; // =1 if switch ON and =0 if OFF 
  uint8_t switch_4; // =1 if switch ON and =0 if OFF 
  uint8_t switch_5; // =1 if switch ON and =0 if OFF 
  uint8_t switch_6; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_SWITCH_1 2
#define PIN_SWITCH_2 3
#define PIN_SWITCH_3 4
#define PIN_SWITCH_4 5
#define PIN_SWITCH_5 17
#define PIN_SWITCH_6 6


void setup() 
{
  RemoteXY_Init (); 
  
  pinMode (PIN_SWITCH_1, OUTPUT);
  pinMode (PIN_SWITCH_2, OUTPUT);
  pinMode (PIN_SWITCH_3, OUTPUT);
  pinMode (PIN_SWITCH_4, OUTPUT);
  pinMode (PIN_SWITCH_5, OUTPUT);
  pinMode (PIN_SWITCH_6, OUTPUT);
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_2, (RemoteXY.switch_2==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_3, (RemoteXY.switch_3==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_4, (RemoteXY.switch_4==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_5, (RemoteXY.switch_5==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_6, (RemoteXY.switch_6==0)?LOW:HIGH);
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 


}
