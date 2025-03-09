// int incomingByte = 0; 
// int a[9] = {0};
// int c = 0;
// void setup() {
//   Serial.begin(9600);
//   Serial.println("START");  
// }
 
// void loop() {
//       delay(5);
//       Serial.flush();
//       if(Serial.available() > 0){
//         Serial.println("#####################################");
//         for (int i=0; i<9; i++){
//             incomingByte = Serial.read();
//             Serial.print(i);
//             Serial.print(" read: ");
//             Serial.println(incomingByte);
//             delay(20);
//             a[i] = incomingByte; 
// //            Serial.println(a[i]);                  
//          } 
//          c = a[4]*256 + a[5];
//          Serial.print("甲醛：");  
//          Serial.println(c);        
//       }
//   }

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11);//rx,tx;

int incomingByte = 0; 
int a[9] = {0};
int c = 0;  

void setup() {
  // 开启硬件串行端口与电脑通信
  Serial.begin(9600);
  // 开启软件串行端口与传感器通信
  mySerial.begin(9600);
  Serial.println("ZE08-CH2O sensor reading...");
}

void loop() {

  delay(5);
  if(mySerial.available() > 0){
      for (int i=0; i<9; i++){
        incomingByte = mySerial.read();
        mySerial.print(i);
        Serial.print("read:");
        Serial.println(incomingByte);
        delay(20);
        a[i] = incomingByte; 
//            Serial.println(a[i]);                  
      } 
      c = a[4]*256 + a[5];
      Serial.print("甲醛：");  
      Serial.println(c);
      Serial.println();
        
  }
  Serial.println(1);
  delay(5000);
}

  