#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT
#include <RemoteXY.h>                                                 // thư viện cho remoteXY - công cụ hỗ trợ
#define REMOTEXY_SERIAL Serial                                      // thư viện cho remoteXY - công cụ hỗ trợ 
#define REMOTEXY_SERIAL_SPEED 115200                              // thiết lập truyền
#define REMOTEXY_WIFI_SSID "Smart Home"                          // đặt tên cho wifi 
#define REMOTEXY_WIFI_PASSWORD "12345678"                       // mật khẩu truy cập wifi 
#define REMOTEXY_SERVER_PORT 6377                              // dùng port mặc định của remote Xy 

#include <SPI.h>                                             // Include các thư viện cần thiết
#include <MFRC522.h> 
#include <Servo.h> 
#include <Wire.h>  
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <IRremote.h> 

#define SERVO_PIN 13                                          // pin cho servo Giàn phơi đồ thông minh 
int cambienanhsang = 12;                                     // thiết lập cảm biến sánh sáng phơi đồ
int cambienmua = 11;                                        // thiết lập cảm biến nhận tín hiệu mưa

Servo gServo;                                             // Đối tượng thực hiện servo phơi đồ
Servo myservo,myservo1;                                   // đối tượng cho servo cửa chính
Servo servocong,servocong1;                               // đối tượng cho servo cổng

int pos = 0;                                              // giá trị cho servo 
#define SS_PIN 10                                         //chân cho RFID
#define RST_PIN 9                                         //chân cho RFID

#define MotorInterfaceType 8                             // khai báo kiểu cho động cơ bước
IRrecv irrecv(38);                                        // đặt chân nhận dữ liệu là 38
decode_results ketquaRemote;
int gianphoi = 0;



                        // ***************************khu vực cho password mở cổng*****************************************//
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
byte rowPins[ROWS] = {22, 23, 24, 25}; // khai báo key matrix dạng 4x4 và các chân tương ứng
byte colPins[COLS] = {26, 27, 28, 29}; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
LiquidCrystal_I2C lcd(0x27, 16, 2);

//************************************************************************ kết thúc khu vực cổng *******************************************************//


MFRC522 mfrc522(SS_PIN, RST_PIN);      // khai báo RFID
unsigned long uidDec, uidDecTemp; // hien thi so UID dang thap phan
byte bCounter, readBit;
unsigned long ticketNumber;
int tam = 0;

//**************************************************************** RemoteXY configurate  : Khai báo cho remotexy
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,17,0,0,0,235,1,10,13,1,
  2,1,3,13,17,8,2,26,31,179,
  79,78,0,79,70,70,0,129,0,6,
  8,11,3,17,196,144,195,168,110,32,
  80,75,0,2,1,24,13,17,8,2,
  26,31,179,79,78,0,79,70,70,0,
  129,0,27,8,13,3,17,196,144,195,
  168,110,32,80,78,49,0,2,1,44,
  13,17,8,2,26,31,179,79,78,0,
  79,70,70,0,129,0,47,8,13,3,
  17,196,144,195,168,110,32,80,78,50,
  0,2,1,2,29,17,8,2,26,31,
  179,79,78,0,79,70,70,0,129,0,
  5,24,11,3,17,196,144,195,168,110,
  32,66,225,186,191,112,0,2,1,23,
  29,17,8,2,26,31,179,79,78,0,
  79,70,70,0,129,0,26,24,13,3,
  17,196,144,195,168,110,32,67,84,0,
  2,1,43,29,17,8,2,26,31,179,
  79,78,0,79,70,70,0,129,0,46,
  24,13,3,17,196,144,195,168,110,32,
  86,83,0,2,1,2,45,17,8,2,
  26,31,179,79,78,0,79,70,70,0,
  129,0,5,40,12,3,17,81,117,225,
  186,161,116,32,80,75,0,2,1,23,
  45,17,8,2,26,31,179,79,78,0,
  79,70,70,0,129,0,26,40,14,3,
  17,81,117,225,186,161,116,32,66,225,
  186,191,112,0,2,1,43,45,17,8,
  2,26,31,179,79,78,0,79,70,70,
  0,129,0,46,40,12,3,17,196,144,
  195,168,110,32,83,195,162,110,0,1,
  6,3,66,9,9,2,31,196,144,195,
  179,110,103,0,129,0,3,59,22,6,
  17,67,225,187,173,97,32,80,75,0,
  1,6,15,66,9,9,2,31,77,225,
  187,159,0,1,6,35,66,9,9,2,
  31,196,144,195,179,110,103,0,129,0,
  32,59,28,6,17,67,225,187,173,97,
  32,67,225,187,149,110,103,0,1,6,
  47,66,9,9,2,31,77,225,187,159,
  0,1,6,3,87,9,9,2,31,196,
  144,195,179,110,103,0,129,0,3,80,
  22,6,17,71,97,114,97,32,79,116,
  111,0,1,6,15,87,9,9,2,31,
  77,225,187,159,0,1,6,35,87,9,
  9,2,31,196,144,195,179,110,103,0,
  129,0,35,80,23,6,17,80,104,111,
  105,32,81,65,0,1,6,47,87,9,
  9,2,31,77,225,187,159,0 };
  
//**************************************************************** this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  uint8_t switch_2; // =1 if switch ON and =0 if OFF 
  uint8_t switch_3; // =1 if switch ON and =0 if OFF 
  uint8_t switch_4; // =1 if switch ON and =0 if OFF 
  uint8_t switch_5; // =1 if switch ON and =0 if OFF 
  uint8_t switch_6; // =1 if switch ON and =0 if OFF 
  uint8_t switch_7; // =1 if switch ON and =0 if OFF 
  uint8_t switch_8; // =1 if switch ON and =0 if OFF 
  uint8_t switch_9; // =1 if switch ON and =0 if OFF 
  uint8_t button_1; // =1 if button pressed, else =0 
  uint8_t button_2; // =1 if button pressed, else =0 
  uint8_t button_3; // =1 if button pressed, else =0 
  uint8_t button_4; // =1 if button pressed, else =0 
  uint8_t button_5; // =1 if button pressed, else =0 
  uint8_t button_6; // =1 if button pressed, else =0 
  uint8_t button_7; // =1 if button pressed, else =0 
  uint8_t button_8; // =1 if button pressed, else =0 
   // other variable
  uint8_t connect_flag;  

} RemoteXY;

//************************ Khai Báo các chân Pin Được thiết lập sẵn ********************************************************
#pragma pack(pop)
#define PIN_SWITCH_1 2
#define PIN_SWITCH_2 3
#define PIN_SWITCH_3 4
#define PIN_SWITCH_4 5
#define PIN_SWITCH_5 17
#define PIN_SWITCH_6 6
#define PIN_SWITCH_7 7
#define PIN_SWITCH_8 8
#define PIN_SWITCH_9 16
#define PIN_BUTTON_1 A15
#define PIN_BUTTON_2 A14
#define PIN_BUTTON_3 A13
#define PIN_BUTTON_4 A12
#define PIN_BUTTON_5 A11
#define PIN_BUTTON_6 A10
#define PIN_BUTTON_7 A9
#define PIN_BUTTON_8 A8

AccelStepper stepper = AccelStepper(MotorInterfaceType, PIN_BUTTON_6, 32, PIN_BUTTON_5, 33); /// khai báo cho động cơ bước

// **************************************** hàm setup các chân và đầu vào ra cho các thiết bị **********************************************************
void setup() 
{
  //************************************** khai báo khởi tạo và các chân output cho các đèn trong nhà
  RemoteXY_Init (); 
  pinMode (PIN_SWITCH_1, OUTPUT);
  pinMode (PIN_SWITCH_2, OUTPUT);
  pinMode (PIN_SWITCH_3, OUTPUT);
  pinMode (PIN_SWITCH_4, OUTPUT);
  pinMode (PIN_SWITCH_5, OUTPUT);
  pinMode (PIN_SWITCH_6, OUTPUT);
  pinMode (PIN_SWITCH_7, OUTPUT);
  pinMode (PIN_SWITCH_8, OUTPUT);
  pinMode (PIN_SWITCH_9, OUTPUT);
  pinMode (PIN_BUTTON_1, OUTPUT);
  pinMode (PIN_BUTTON_2, OUTPUT);
  pinMode (PIN_BUTTON_3, OUTPUT);
  pinMode (PIN_BUTTON_4, OUTPUT);
  pinMode (PIN_BUTTON_5, OUTPUT);
  pinMode (PIN_BUTTON_6, OUTPUT);
  pinMode (PIN_BUTTON_7, OUTPUT);
  pinMode (PIN_BUTTON_8, OUTPUT);

  // ****************************************************** phần cửa chính dùng rfid ***************************************************
  SPI.begin();            
  mfrc522.PCD_Init();     
  myservo.attach(A6);
  myservo1.attach(A7);
  
//********************************************************** phần giàn phơi thông minh sử dụng cảm biến mưa và ánh sáng ********************************* 
  pinMode(cambienanhsang,INPUT);
  pinMode(cambienmua,INPUT);
  gServo.attach(SERVO_PIN);

// +*****************************************************phần lcd và mở cổng *********************************************************
  lcd.backlight();
  lcd.init();
  servocong.attach(30);
  servocong1.attach(31);
  //***********************************************Thiết lập cho động cơ bước****************************************
  stepper.setMaxSpeed(1000);
  irrecv.enableIRIn();
  
}

void loop() 
{ 
  ReadRemote();// Gọi Hàm Đọc Remote
  MoCong(); //******************************************************** thực hiện gọi hàm mở cổng
  RemoteXY_Handler ();
  digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_2, (RemoteXY.switch_2==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_3, (RemoteXY.switch_3==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_4, (RemoteXY.switch_4==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_5, (RemoteXY.switch_5==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_6, (RemoteXY.switch_6==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_7, (RemoteXY.switch_7==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_8, (RemoteXY.switch_8==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_9, (RemoteXY.switch_9==0)?LOW:HIGH);
  digitalWrite(PIN_BUTTON_1, (RemoteXY.button_1==0)?LOW:HIGH);
  if(RemoteXY.button_1==1){
   DongMoCuaDai(123);
  }
  digitalWrite(PIN_BUTTON_2, (RemoteXY.button_2==0)?LOW:HIGH);
  if(RemoteXY.button_2==1){
  DongMoCuaDai(123);
  }
  digitalWrite(PIN_BUTTON_3, (RemoteXY.button_3==0)?LOW:HIGH);
  if(RemoteXY.button_3==1){
    //MoCongWifi();
    DongCongWifi();
  }
  digitalWrite(PIN_BUTTON_4, (RemoteXY.button_4==0)?LOW:HIGH);
  if(RemoteXY.button_4==1){
    //DongCongWifi();
    MoCongWifi();
  }
  digitalWrite(PIN_BUTTON_5, (RemoteXY.button_5==0)?LOW:HIGH);
  if(RemoteXY.button_5==1){
      MoGara();
  }
  digitalWrite(PIN_BUTTON_6, (RemoteXY.button_6==0)?LOW:HIGH);
    if(RemoteXY.button_6==1 ){
      DongGara();
    }
  
  digitalWrite(PIN_BUTTON_7, (RemoteXY.button_7==0)?LOW:HIGH);
  digitalWrite(PIN_BUTTON_8, (RemoteXY.button_8==0)?LOW:HIGH);
  
  RFIDMoCua();// **********************************************thực hiện gọi hàm kiểm tra thẻ từ RFID ***************************************************************
  PhoiDo();
}
//***************************************************** KẾT THỨC VÒNG LẶP LOOP *****************************************************************************************






// **************************************** Hàm đọc Giá trị từ remote*************************************
void ReadRemote(){
  if(irrecv.decode(&ketquaRemote)){
  irrecv.resume();
    if(ketquaRemote.value == 16753245){
      MoCongWifi();
    }
    if(ketquaRemote.value == 16769565){
      DongCongWifi();
    }
    if(ketquaRemote.value == 16720605){
       DongMoCuaDai(123);
    }
 }

  // đèn Gara
  if(ketquaRemote.value == 16712445){
       digitalWrite(16,HIGH);
    }
  if(ketquaRemote.value == 16761405){
       digitalWrite(16,LOW);
    }
   // đèn phòng khách
   if(ketquaRemote.value == 16769055){
       digitalWrite(2,HIGH);
    }
  if(ketquaRemote.value == 16754775){
       digitalWrite(2,LOW);
    }
   // đèn bếp
   if(ketquaRemote.value == 16748655){
       digitalWrite(5,HIGH);
    }
  if(ketquaRemote.value == 16738455){
       digitalWrite(5,LOW);
    }
  // đèn ngủ 1
  if(ketquaRemote.value == 16750695){
       digitalWrite(3,HIGH);
    }
  if(ketquaRemote.value == 16756815){
       digitalWrite(3,LOW);
    }
   // đèn ngủ 2
   if(ketquaRemote.value == 16724175){
       digitalWrite(4,HIGH);
    }
  if(ketquaRemote.value == 16718055){
       digitalWrite(4,LOW);
    }
  // đèn Vệ Sinh 
  if(ketquaRemote.value == 16743045){
       digitalWrite(6,HIGH);
    }
  if(ketquaRemote.value == 16743045){
       digitalWrite(6,LOW);
    }
   // bật quạt khách
   if(ketquaRemote.value == 16726215){
       digitalWrite(7,HIGH);
    }
  if(ketquaRemote.value == 16734885){
       digitalWrite(7,LOW);
    }
    // bât quạt bếp
       if(ketquaRemote.value == 16728765){
       digitalWrite(8,HIGH);
    }
  if(ketquaRemote.value == 16730805){
       digitalWrite(8,LOW);
    }
// ALL LAPMLE 
   if(ketquaRemote.value == 16732845){
       digitalWrite(2,HIGH);
       digitalWrite(3,HIGH);
       digitalWrite(4,HIGH);
       digitalWrite(5,HIGH);
    }



    
}
// ****************************************ẸND Hàm đọc Giá trị từ remote*************************************








// ***************************************** Hàm Thực hiện đóng mở gara Oto*************************************
void DongGara(){
    stepper.setCurrentPosition(0);
  while (stepper.currentPosition() != 768) {
    stepper.setSpeed(1000);
    stepper.runSpeed();
  }
}
void MoGara(){
    stepper.setCurrentPosition(0);
     while (stepper.currentPosition() != -768) {
    stepper.setSpeed(-1000);
    stepper.runSpeed();
     }
}

// *****************************************End Hàm Thực hiện đóng mở gara Oto*************************************









// *********************************************hàm thực hiện mở cổng qua wifi ************************************
void MoCongWifi(){
      servocong1.write(90);
      delay(600);
      servocong.write(120);
  
}
void DongCongWifi(){
      servocong1.write(0);
      delay(600);
    servocong.write(5);
}
// ********************************************* End hàm thực hiện mở cổng qua wifi ************************************










//*************************************************** hàm để mở cổng dùng password *************************************************************************************
void MoCong(){
  lcd.setCursor(0, 0); // thiết lập vị trí hiển trị trên lcd
  lcd.print("=>Nhap Password:"); // hiển thị câu lệnh
  customKey = customKeypad.getKey(); // nhận dữ liệu từ keypad từng phím một 
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
      servocong1.write(90);
      delay(600);
      servocong.write(120);
      delay(7000);
         servocong1.write(0);
          delay(600);
        servocong.write(5);
         
    }
    else {
      lcd.print("Sai PassWord");
      delay(1000);
           servocong1.write(0);
            delay(600);
          servocong.write(5);
    }
    lcd.clear();
    clearData();
  }
}
//******************************************************************************** end mở cổng ************************************





//************************************************************************** hàm xóa dữ liệu của mở cổng *****************************
void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
//***************************************************************************end hàm xóa dữ liệu *************************************








// ******************************************************************************* hàm phơi đồ thông minh******************************
void PhoiDo(){
  int value = digitalRead(cambienmua);
   if(RemoteXY.button_7==1){
    gianphoi =1;
     }else if(RemoteXY.button_8==1){
      gianphoi = 2;
     }
     // kiem tra tin hieu 

     if(value == HIGH){
                    
                     gServo.write(0);
                    int anhsang  = digitalRead(cambienanhsang);
                    if(anhsang == 1){
                       gServo.write(180);
                      
                    }else{
                       if(gianphoi == 1){
                          gServo.write(180);
                          }else if(gianphoi ==2){
                           gServo.write(0);
                      }
                      gianphoi = 0;
                      gServo.write(0);
                    }
                    }else{  
                      gianphoi = 0;
                      gServo.write(180);
                       
                    }
         if(gianphoi == 1){
            gServo.write(180);
            }else if(gianphoi ==2){
             gServo.write(0);
        }
         
   
}
//**********************************************************************************end phơi đồ **********************************










// *****************************************************************************ham dong mở của chính
void DongMoCuaDai(int nhanDuLieu){
  if(nhanDuLieu == 123){
    tam++;
    delay(1000);
     if(tam == 1){
       for(pos = 0; pos<= 180 ; pos++) {                             
        myservo.write(pos);
        myservo1.write(170-pos);
        delay(15);
    } 
  }else if (tam == 2){
       for(pos = 180; pos>=0 ; pos--) {                             
        myservo.write(pos);
         myservo1.write(170-pos);
        delay(15);
    }  
    tam = 0;
  }
    
  }
}
//*********************************************************************************end đóng mở của chính











// *************************************************************************************ham doc thẻ RFID
void RFIDMoCua(){
    // Tim the moi
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Doc the
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  uidDec = 0;
  // Hien thi so UID cua the
 // Serial.println("Serijnyj nomer karty / Card UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec*256+uidDecTemp;
  } 
  //Serial.print("            [");
  //Serial.print(uidDec);
  if(uidDec == 2031997368){
    tam++;
    delay(1000);
     if(tam == 1){
       for(pos = 0; pos<= 180 ; pos++) {                             
        myservo.write(pos);
        myservo1.write(170-pos);
        delay(15);
    } 
  }else if (tam == 2){
       for(pos = 180; pos>=0 ; pos--) {                             
        myservo.write(pos);
         myservo1.write(170-pos);
        delay(15);
    }  
    tam = 0;
  }
    
  }else{
  }
  //--------------------------------
  if(uidDec == 350875435){
    tam ++;
    delay(1000);
  if(tam == 1){
       for(pos = 0; pos<= 180 ; pos++) {                             
        myservo.write(pos);
        myservo1.write(170-pos);
        delay(15);
    } 
  }else if (tam == 2){
       for(pos = 180; pos>=0 ; pos--) {                             
        myservo.write(pos);
         myservo1.write(170-pos);
        delay(15);
    }  
    tam = 0;
  }
    
  }else{
  }
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak); // Tra cuu dinh dang the
  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL) { // Neu khong dung dinh dang the

    mfrc522.PICC_HaltA();  // dung lai
    return;
    delay(1);
  }

  // Neu dung dinh dang the
  byte status;
  byte byteCount;
  byte buffer[18]; // do dai (16 byte + 2 byte dieu khien). 
  byte pages[2] = {
    4, 8                      }; // so trang co du lieu.

  byte pageByte; // dem so byte trong trang

  byteCount = sizeof(buffer);
  byte bCount = 0;


  for (byte i = 0; i<2; i++) { // bat dau doc trang
    status = mfrc522.MIFARE_Read(pages[i], buffer, &byteCount);

    if (status != MFRC522::STATUS_OK) {
    }
    else {
      if (pages[i] == 4) {
        bCounter = 0; // bo dem 32-bit

        // bit 0-3.
        for (bCount = 0; bCount < 4; bCount++) {
          readBit = bitRead(buffer[6], (bCount+4));
          setBitsForGood(readBit);
        }

        // bit 4 - 27.
        for (pageByte = 5; pageByte > 2; pageByte--) {
          for (bCount = 0; bCount<8; bCount++) {
            readBit = bitRead(buffer[pageByte], bCount);
            setBitsForGood(readBit);
          }
        }

        // bit 28-31.
        for (bCount = 0; bCount < 4; bCount++) {
          readBit = bitRead(buffer[2], bCount);
          setBitsForGood(readBit);
        }
      }
      if (pages[i] == 8) {
        unsigned int issueDate = buffer[0] * 256 + buffer[1];
        printIssueDate(issueDate);
      }
    }
  }
  mfrc522.PICC_HaltA(); 
}

void printIssueDate(unsigned int incoming) {

  boolean isLeap = true; // kiem tra nam nhuan
  int days[]={
    0,31,59,90,120,151,181,212,243,273,304,334                      };
  byte dayOfMonth, monthCounter;
  unsigned int yearCount;
  for (yearCount = 1992; incoming >366; yearCount++) { 

    if ((yearCount % 4 == 0 && yearCount % 100 != 0) ||  yearCount % 400 == 0) {
      incoming = incoming - 366;
      isLeap = true;
    } 
    else {
      incoming = incoming - 365;
      isLeap = false;
    }
  }
  // tinh so thu tu thang
  for (monthCounter = 0; incoming > days[monthCounter]; monthCounter++) {
  }
  if (isLeap == true) { // neu nam nhuan
    if (days[monthCounter-1]>31) { 
      dayOfMonth = incoming - (days[monthCounter-1]+ 1);
    } 
    else {
      dayOfMonth = incoming - (days[monthCounter-1]); 
    }
  }
  else {
    dayOfMonth = incoming - (days[monthCounter-1]); // neu khong phai nam nhuan
  }
}

void setBitsForGood(byte daBeat) {
  if (daBeat == 1) {
    bitSet(ticketNumber, bCounter);
    bCounter=bCounter+1;
  }
  else {
    bitClear(ticketNumber, bCounter);
    bCounter=bCounter+1;
  }
}
