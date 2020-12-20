#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>                      //Thư viện LCD i2c
#include <LiquidCrystal.h>
int speaker = 4;                 // loa,den ket noi Pin 4
int gas = 2;                    // out ket noi Pin 2
int led = 3;// led he thong bao chay
int fan1 = 8;
int fan2 = 9;
#include "DHT.h"
#define DHTPIN 6  
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

const byte DS1307 = 0x68;                           //Địa chỉ DS1307
const byte NumberOfFields = 7;                      //Số byte cần đọc
int second, minute, hour, day, wday, month, year;   //Biến lưu giá trị 
char daysOfTheWeek[7][12] = {"CN ", "Hai ", "Ba ", "Tu ", "Nam ", "Sau ", "Bay "}; // mảng các thứ trong tuần 
int i = 0;

LiquidCrystal_I2C lcd(0x27,16,2);                   //Khai báo LCD1602
RTC_DS1307 rtc;
void setup()
{
  pinMode(gas, INPUT);
  pinMode(speaker, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(fan1,OUTPUT);
  pinMode(fan2,OUTPUT);
  Wire.begin();                                     //Khai báo giao thức I2C
  lcd.init();                                       //Khởi tạo LCD
  lcd.backlight();                                  //Bật đèn nền
  lcd.clear();                                      //Xóa màn hình LCD
  Serial.begin(9600);                               //Bật cổng Serial 9600
  dht.begin();                                     // Khởi động DHT11
}

void loop()
{
  HeThongBaoChay();
 if(i<6){
   if (!Serial.available())                           //Nếu không có tín hiệu Serial
  { 
    ReadDS1307();                                    //Đọc từ RTC
    DisplayDS1307();                                 //Hiển thị lên LCD
    delay(1000);  
  }
  else                                               //Ngược lại
  {
    ReadSerial();                                    //Đọc giá trị Serial
  }
 }
 
 if(i>6 && i<15){
  ReadDHT();
 }

 if(i==15){
  i =0;
 }
  i++;
 
}

void HeThongBaoChay(){
   if (digitalRead(gas) == HIGH)
  {
   for(int i = 0 ; i< 30; i++){
    lcd.setCursor(0,0); 
    lcd.print("   Bao Dong!   ");
    lcd.setCursor(0,1); 
    lcd.print("   Ro Khi Ga   ");
    digitalWrite(led, HIGH);
    digitalWrite(speaker, HIGH);
    delay(65 );
    digitalWrite(led, LOW);
    digitalWrite(speaker, LOW);
    delay(100);
   }
   i = 0;
  }
  else
  {
    digitalWrite(led, LOW);
    digitalWrite(speaker, LOW);
  }
}
void ReadDHT(){
  float  h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h)) {
    lcd.setCursor(0,0); 
    lcd.print("loi doc tu dht11");
  } else {
    lcd.setCursor(0,0); 
    lcd.print("Do Am:    ");
    lcd.print(h);
    lcd.setCursor(0,1); 
    lcd.print("Nhiet Do: ");
    lcd.print(t);
    if(t >32){
      digitalWrite(fan1,HIGH); // TỰ ĐỘNG KHỞI ĐỘNG HỆ THỐNG LÀM MÁT 
      digitalWrite(fan2,HIGH);
    }else{
      digitalWrite(fan1,LOW); // NGẮT HỆ THỐNG LÀM MÁT
      digitalWrite(fan2,LOW);
    }
    delay(100);
  }
}
void ReadSerial()
{
    byte hr; byte min; byte sec; byte wd; byte d; byte mth; byte yr;    //Biến lưu
    String buff = Serial.readStringUntil('\n');                         //Đọc đến khi hết dòng
    String tmp;                                                         //Biến tạm để tách  
    
    for (int i=0;i<7;i++)                                               //Đọc vào 7 thông tin theo thứ tự (Giờ-Phút-Giây-Thứ trong tuần-Ngày-Tháng-Năm)
    {
      int index=buff.indexOf(" ");                                      //Lấy vị trí đến dấu cách
      tmp=buff.substring(0,index);                                      //Tách biến tạm
      buff=buff.substring(index+1,buff.length());                       //Tính lại buff
      switch (i)                                                        //Đọc dữ liệu và gán vào
      {
        case 0:
          hr=tmp.toInt();
          break;
        case 1:
          min=tmp.toInt();
          break;
        case 2:
          sec=tmp.toInt();
          break;
        case 3:
          wd=tmp.toInt();
          break;
        case 4:
          d=tmp.toInt();
          break;
        case 5:
          mth=tmp.toInt();
          break;
        case 6:
          yr=tmp.toInt();
          break;
      }
    }
    SetTime(hr, min, sec, wd, d, mth, yr);                    //Thiết lập lại thời gian
}

void ReadDS1307()
{
  Wire.beginTransmission(DS1307);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom(DS1307, NumberOfFields);
  DateTime now = rtc.now();
  second = now.second();
  minute = now.minute();
  hour   = now.hour() ;
  wday   = daysOfTheWeek[now.dayOfTheWeek()];
  day    = now.day();
  month  = now.month();
  year   = now.year();
 
}

int bcd2dec(byte num) { return ((num / 16 * 10) + (num % 16)); } // Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal
int dec2bcd(byte num) { return ((num / 10 * 16) + (num % 10)); } // Chuyển từ Decimal sang BCD

void DisplayDS1307()
{
  //Xuất ra LCD
  DateTime now = rtc.now(); // khởi tạo hàm thời gian date
  lcd.clear();              //Xóa màn hình
  lcd.setCursor(0,0);       //Đặt con trỏ về 0,0
  lcd.print("Gio ");
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
  lcd.setCursor(0,1);       //Đặt con trỏ về 0,1
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print(day);
  lcd.print("/");
  lcd.print(month);
  lcd.print("/");
  lcd.print(year);
}

void SetTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
  Wire.beginTransmission(DS1307); //Bắt đầu truyền đến địa chỉ DS1307
  Wire.write(byte(0x00));         //Đặt lại con trỏ
  Wire.write(sec);
  Wire.write(min);
  Wire.write(hr);
  Wire.write(wd);        //Ngày trong tuần: 1 = Chủ nhật; 7 = Thứ bảy
  Wire.write(d);
  Wire.write(mth);
  Wire.write(yr);
  Wire.endTransmission();         //Ngắt tín hiệu
}
