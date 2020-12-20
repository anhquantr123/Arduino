#include <SPI.h>
#include <MFRC522.h> 
// thu vien "RFID".
#include <Servo.h> 
#include <Wire.h>    
Servo myservo; 
Servo myservo1; 

int pos = 0;
const int LED1 = 6; 
int speaker = 4;                 // loa,den ket noi Pin 4
int gas = 2;                    // out ket noi Pin 2
int led = 3;// led he thong bao chay 

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);       
unsigned long uidDec, uidDecTemp; // hien thi so UID dang thap phan
byte bCounter, readBit;
unsigned long ticketNumber;

int tam = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);     
  SPI.begin();            
  mfrc522.PCD_Init();     
  myservo.attach(8);
   myservo1.attach(7);
   pinMode(gas, INPUT);
  pinMode(speaker, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.println("Prilozhite kartu / Waiting for card...");
}

void loop() {
    // vong lap cho he thong bao chay
  if (digitalRead(gas) == HIGH)
  {
   for(int i = 0 ; i< 20; i++){
    digitalWrite(led, HIGH);
    digitalWrite(speaker, HIGH);
    delay(80);
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
  // Tim the moi
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Doc the
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  uidDec = 0;
  Serial.println("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM");
  Serial.println("================================================");

  // Hien thi so UID cua the
  Serial.println("Serijnyj nomer karty / Card UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec*256+uidDecTemp;
  } 
  Serial.print("            [");
  Serial.print(uidDec);
  if(uidDec == 2031997368){
    tam++;
    digitalWrite(LED1,!digitalRead(LED1));
    delay(1000);
     if(tam == 1){
       for(pos = 0; pos<= 180 ; pos++) {                             
        myservo.write(pos);
        myservo1.write(180-pos);
        delay(15);
    } 
  }else if (tam == 2){
       for(pos = 180; pos>=0 ; pos--) {                             
        myservo.write(pos);
         myservo1.write(180-pos);
        delay(15);
    }  
    tam = 0;
  }
    digitalWrite(LED1,LOW);
  }else{
  }
  //--------------------------------
  if(uidDec == 350875435){
    tam ++;
    digitalWrite(LED1,!digitalRead(LED1));
    delay(1000);
  if(tam == 1){
       for(pos = 0; pos<= 180 ; pos++) {                             
        myservo.write(pos);
        myservo1.write(180-pos);
        delay(15);
    } 
  }else if (tam == 2){
       for(pos = 180; pos>=0 ; pos--) {                             
        myservo.write(pos);
         myservo1.write(180-pos);
        delay(15);
    }  
    tam = 0;
  }
    digitalWrite(LED1,LOW);
  }else{
  }
  Serial.println("]");
  Serial.println("================================================");

  // Hien thi loai the
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak); // Tra cuu dinh dang the
  Serial.println("Tip karty / Card type: ");

  Serial.print(" [");
  Serial.print(mfrc522.PICC_GetTypeName(piccType)); // Chuyen dinh dang the ve kieu doc
  Serial.println("]");
  Serial.println("================================================");
  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL) { // Neu khong dung dinh dang the

    Serial.println("Neizvestnaja karta / Not a valid card: Type"); 
    Serial.print("            [");
    Serial.print(piccType); 
    Serial.println("]");  
    Serial.println("================================================");
    Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"); 

    // Halt PICC
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
      Serial.print("Read error: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
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

        Serial.println("Napechatannyj nomer na karte / Ticket number: ");
        Serial.print("            [");
        Serial.print(ticketNumber, DEC);
        Serial.println("]");
        Serial.println("================================================");
      }

      if (pages[i] == 8) { // ngay san xuat

        Serial.println("Data pokupki karty / Issued: ");
        Serial.println("    Chislo Mesjac God");
        // so ngay tu 01.01.1992 dang thap phan
        unsigned int issueDate = buffer[0] * 256 + buffer[1];
        printIssueDate(issueDate);
        Serial.println("================================================");

        Serial.println("Srok dejstvija karty / Good for (days): "); // han su dung
        Serial.print("            [");
        Serial.print(buffer[2], DEC);                                                           
        Serial.print("]");  
        Serial.println(" Ngay");        
        Serial.println("================================================");
        Serial.println("Kolichestvo ostavshihsja poezdok / Trip reminder: ");
        Serial.print("            [");
        // So lan su dung con lai
        Serial.print(buffer[5], DEC);                                                           
        Serial.println("]");  

        Serial.println("================================================");
        Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");      
      }
    }
  }
  // Halt PICC
  mfrc522.PICC_HaltA();                       

}

void printIssueDate(unsigned int incoming) {

  boolean isLeap = true; // kiem tra nam nhuan
  int days[]={
    // cac ngay cuoi cung trong thang theo thu tu doi voi nam binh thuong
    0,31,59,90,120,151,181,212,243,273,304,334                      };
  byte dayOfMonth, monthCounter;
  unsigned int yearCount;

  // bat dau tinh tu 01.01.1992
  //  incoming = incoming+1; 

  // tinh nam, so ngay ke tu khi san xuat
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

  // tinh so thu tu ngay trong thang

  if (isLeap == true) { // neu nam nhuan

    // neu khong phai thang dau tien, thi them 1 vao ngay cuoi cung cua thang
    if (days[monthCounter-1]>31) { 
      dayOfMonth = incoming - (days[monthCounter-1]+ 1);
    } 
    else {
      dayOfMonth = incoming - (days[monthCounter-1]); 
    }
  }
  // neu la thang dau tien
  else {
    dayOfMonth = incoming - (days[monthCounter-1]); // neu khong phai nam nhuan
  }
  Serial.print("            [");
  Serial.print(dayOfMonth);
  Serial.print(".");
  Serial.print(monthCounter);
  Serial.print(".");
  Serial.print(yearCount);
  Serial.println("]"); 
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
