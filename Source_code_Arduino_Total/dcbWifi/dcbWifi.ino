// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

#include <AccelStepper.h>

#define motorPin3  10     // IN3 on the ULN2003 driver
#define motorPin4  11     // IN4 on the ULN2003 driver

#define MotorInterfaceType 8
// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,2,0,0,0,37,0,10,119,1,
  2,0,4,23,22,11,2,26,31,31,
  79,78,0,79,70,70,0,2,0,35,
  22,22,11,2,26,31,31,79,78,0,
  79,70,70,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  uint8_t switch_2; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_SWITCH_1 8
#define PIN_SWITCH_2 9

AccelStepper stepper = AccelStepper(MotorInterfaceType, PIN_SWITCH_1, motorPin3, PIN_SWITCH_2, motorPin4);
void setup() 
{
  RemoteXY_Init (); 
  pinMode (PIN_SWITCH_1, OUTPUT);
  pinMode (PIN_SWITCH_2, OUTPUT);
  stepper.setMaxSpeed(1000);`
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1==0)?LOW:HIGH);
    if(RemoteXY.switch_1==1){
    stepper.setCurrentPosition(0);
  while (stepper.currentPosition() != 768) {
    stepper.setSpeed(1000);
    stepper.runSpeed();
  }
  }
  digitalWrite(PIN_SWITCH_2, (RemoteXY.switch_2==0)?LOW:HIGH);
    if(RemoteXY.switch_2==1 ){
    stepper.setCurrentPosition(0);
  while (stepper.currentPosition() != -768) {
    stepper.setSpeed(-1000);
    stepper.runSpeed();
  }
    }

}
