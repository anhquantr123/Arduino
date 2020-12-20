#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>     
Servo servocong,servocong1; 
#define Password_Length 8
char Data[Password_Length];
char Master[Password_Length] = "123A456";
byte data_count = 0;
char customKey;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 23, 24, 25}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {26, 27, 28, 29}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup(){
  lcd.backlight();
  lcd.init();
  servocong.attach(10);
}
  
void loop(){
 lcd.setCursor(0, 0);
  lcd.print("=>Nhap Password:");
 
  customKey = customKeypad.getKey();
  if (customKey) {
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print("*");
    data_count++;
  }
  if (data_count == Password_Length - 1) {
    lcd.clear();
 
    if (!strcmp(Data, Master)) {
      lcd.print("OK Mo Cong");
       servocong.write(180);
      delay(5000);
       servocong.write(0);
    }
    else {
      lcd.print("Sai PassWord");
      delay(1000);
    }

    lcd.clear();
    clearData();
  }
}
 
void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
