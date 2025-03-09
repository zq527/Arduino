/**************************************

基于Arduino的室内环境质量监测系统的连线方式

***************
蜂鸣器
VCC------------------Arduino 5v
GND------------------Arduino GND
I/O------------------Arduino 3
***************

***************
ZE08-CH2O 甲醛浓度传感器
PIN6----------------Arduino 10
PIN5----------------Arduino 11
PIN4----------------Arduino 5v
PIN3----------------Arduino GND
***************

***************
DHT11 温湿度传感器
VCC----------------Arduino 5v
DAT----------------Arduino 2
GND----------------Arduino GND
***************

***************
LCD1602液晶显示屏（带I2C转接模块）
GND---------------Arduino GND
VCC---------------Arduino 5v
SDA---------------Arduino A4
SCL---------------Arduino A5
***************

***************
GP2Y1014AU 粉尘传感器
电阻右边----------5v
电容负极----------GND
PIN5-------------A3
PIN3-------------12
***************

***************
ESP8266
3V-------------Arduino 3.3V
GND------------Arduino GND
TX-------------Arduino 5
RX-------------Arduino 6
***************

*****************************************/

#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>



//定义针脚
#define DHTPIN 2 
//定义类型，DHT11或者其它
#define DHTTYPE DHT11
//进行初始设置 
DHT dht(DHTPIN, DHTTYPE);


// 设置I2C地址为0x27，16列2行
LiquidCrystal_I2C lcd(0x27, 16, 2); 


//甲醛传感器的配置
SoftwareSerial mySerial(10,11);//rx,tx;

int incomingByte = 0; 
int a[9] = {0};
int c = 0; 


//粉尘传感器的配置
int measurePin = A3;            
int ledPower = 12;              
 
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;
 
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
float dustDensity1=3.22;

 

//f蜂鸣器的配置
int pinBuzzer = 3;                        //管脚D3连接到蜂鸣器模块的信号脚

void hoot()
{
  long frequency = 300;                   //频率, 单位Hz
 
                                        
  tone(pinBuzzer, frequency );           ////用tone()函数发出频率为frequency的波形
  delay(1000);                           //等待1000毫秒
  
  noTone(pinBuzzer);                    //停止发声
  delay(2000);
}

//esp8266的配置
SoftwareSerial esp8266(5, 6); // RX, TX


void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  dht.begin(); //DHT开始工作


  lcd.init();                // 初始化LCD
  lcd.backlight();           // 打开背光

  pinMode(ledPower,OUTPUT);
  digitalWrite(ledPower,LOW);
}

void loop() {
  mySerial.begin(9600);
  lcd.clear();

  // 两次检测之间，要等几秒钟，这个传感器有点慢。
  delay(2000);
  // 读温度或湿度要用250毫秒
  float h = dht.readHumidity();//读湿度
  float t = dht.readTemperature();//读温度，默认为摄氏度
  Serial.print("Humidity: ");//湿度
  Serial.println(h);
  lcd.print(h);
  lcd.print(" ");
  Serial.print("Temperature: ");//温度
  Serial.print(t);
  Serial.println(" ℃ ");
  lcd.print(t);
  lcd.print(" ");
  
  lcd.setCursor(0,1);
  lcd.cursor();
  if(mySerial.available() > 0){
      for (int i=0; i<9; i++){
        incomingByte = mySerial.read();
        Serial.print(i);
        Serial.print("read:");
        Serial.println(incomingByte);
        delay(20);
        a[i] = incomingByte; 
//            Serial.println(a[i]);                  
    }

    c = a[4]*256 + a[5];//单位ppb
    Serial.print("CH3OH：");  
    Serial.println(c);
    Serial.println();
    lcd.print(c);
    lcd.print("    ");
  }
  mySerial.end();

  digitalWrite(ledPower,HIGH);
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin);
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,LOW);
  delayMicroseconds(sleepTime);
 
  // calcVoltage = (voMeasured/256.0)*5000;//转化为mv
  // dustDensity = 0.17*calcVoltage-0.1;//单位为μg/m³
  
  calcVoltage = voMeasured*5000/1024.0;
  dustDensity = 0.17*calcVoltage*2-0.1;  ///个人总结
  if(dustDensity>0)
  {
    dustDensity1 =dustDensity;
  }


 
  Serial.println("Raw Signal Value (0-1023):");
  Serial.println(voMeasured);
 
  Serial.println("Voltage:");
  Serial.println(calcVoltage);
 
  Serial.println("Dust Density:");
  Serial.println(dustDensity1);
  lcd.print(dustDensity1);
  
//蜂鸣器报警规则
  //湿度在夏季40-80合适
  if(h>80&&h<40){
    hoot();
  }

  //温度在夏季22-28
  if(t>28&&t<22){
    hoot();
  }

  //甲醛小于0.08mg//m³，即为小于80μg/m³
  if(c>80){
    hoot();
  }

  //PM2.5小于0.05mg//m³，即为小于50μg/m³
  if(dustDensity>50){
    hoot();
  }


  esp8266.println(h);
  delay(100);
  esp8266.println(t);
  delay(100);
  esp8266.println(c);
  delay(100);
  esp8266.println(dustDensity1);
  delay(100);

  delay(2000);
}