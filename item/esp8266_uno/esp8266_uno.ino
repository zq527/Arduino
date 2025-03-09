// #include <Arduino.h>
// #include <SoftwareSerial.h>

// SoftwareSerial wifiSerial(9,10);


// // ===========配置区域==========


// // ===========配置区域===========

// bool doCmdOk(String data, char *keyword)
// {
//   bool result = false;
//   if (data != "") // 对于tcp连接命令，直接等待第二次回复
//   {
//     wifiSerial.print(data); // 发送AT指令
//     wifiSerial.print("\r\n"); 
//   }
//     // 等待模块回复
//   while (!wifiSerial.available());
//   delay(200);

//   if (wifiSerial.find(keyword))        //返回值判断
//     result = true;
//   else  
//     result = false;
//   while (wifiSerial.available()) 
//     wifiSerial.read();                 //清空串口接收缓存
//   delay(500);                         //指令时间间隔
//   return result;
// }



// void setup()
// {
//   Serial.begin(9600);
//   Serial.println("begin");
//   wifiSerial.begin(115200);  
//   Serial.println("0");
//   while (!doCmdOk(F("AT"), "OK"));
//   Serial.println("1");
//   while (!doCmdOk(F("AT+RST"), "OK"));
//   Serial.println("1");
//   while (!doCmdOk(F("AT+CWMODE=1"), "OK"));
//   Serial.println("2");
//   while(!doCmdOk(F("AT+CWDHCP=1,1"), "OK"));
//   Serial.println("3");
//   while(!doCmdOk(F("AT+CWJAP=\"iQOO 9\",\"zq020527\""), "OK"));
//   Serial.println("4");
//   while(!doCmdOk(F("AT+MQTTUSERCFG=0,1,\"Arduino\",\"74974l7RQM\",\"version=2018-10-31&res=products%2F74974l7RQM%2Fdevices%2FArduino&et=1741680668&method=md5&sign=pDijqtZlBe9j%2F5KJqSkmvA%3D%3D\",0,0,\"\""), "OK"));
//   Serial.println("5");
//   while(!doCmdOk(F("AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1"), "OK")); 
//   Serial.println("6");
//   while(!doCmdOk(F("AT+MQTTSUB=0,\"$sys/74974l7RQM/Arduino/thing/property/post/reply\",1"), "OK")); 
//   Serial.println("7"); 
//   // while(!doCmdOk(F("AT+MQTTPUB=0,\"$sys/74974l7RQM/Arduino/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"power\\\":{\\\"value\\\":23\\\}}}\",0,0"), "OK"));
//   // Serial.println("8"); 
// }

// void loop()
// {
//   char s[256];
//   // int a=random(0,100),b=random(0,100);
  
//   sprintf(s,"AT+MQTTPUB=0,\"$sys/74974l7RQM/Arduino/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"power\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",11);
//   while(!doCmdOk(s, "OK"));
//   Serial.println("8"); 

//   if (wifiSerial.available())
//     Serial.write(wifiSerial.read());
//   if (Serial.available())
//   {
//     wifiSerial.write(Serial.read());
//   }
// }

// #include <SoftwareSerial.h>
// SoftwareSerial esp8266(5, 6); // RX, TX
// void setup() {
//   Serial.begin(9600); // Debugging output on the main serial port
//   esp8266.begin(9600); // Communication with the ESP8266 on the secondary serial port
// }
// int a=0;
// void loop() {
//   // Send data to the ESP8266
//   esp8266.println(a++);
//   esp8266.println(a++);
//   delay(1000);
//   // Read the response from the ESP8266
//   if (esp8266.available()) {
//     Serial.write(esp8266.read());
//   }
// }

// #include <Arduino.h>

// #include <ESP8266WiFi.h>

// void setup() {
//   Serial.begin(9600); // 确保波特率与Arduino一致
//   Serial.println("ESP8266 Ready");
// }

// void loop() {
//   if (Serial.available()) { // 检查是否有数据可读
//     String data = Serial.readStringUntil('\n'); // 读取数据直到换行符
//     Serial.println(data);
//     String data2 = Serial.readStringUntil('\n'); // 读取数据直到换行符
//     Serial.println(data2);
//   }
// }





#include<ESP8266WiFi.h>
#include<PubSubClient.h>
#include <iostream> 
using namespace std;

//SoftwareSerial espSerial(2, 3); // RX, TX

String ssid = "iQOO 9"; // Wi-Fi网络名称
String password = "zq020527"; // Wi-Fi网络密码

const char*mqttHostUrl = "mqtts.heclouds.com";//onenet服务器域名
const int mqttPort = 1883;//端口号
const char*mqttUser = "74974l7RQM";//产品id
const char*mqttPassword = "version=2018-10-31&res=products%2F74974l7RQM%2Fdevices%2FArduino&et=1741680668&method=md5&sign=pDijqtZlBe9j%2F5KJqSkmvA%3D%3D";//密钥
const char*clientId = "Arduino";//设备名称
int postMsgId= 0 ;


#define ONENET_POST "$sys/74974l7RQM/Arduino/thing/property/post"
//#define ONENET_FORMAT "{\"id\":\"123\",\"params\":{\"power\":{\"value\":%d}}}"
//#define ONENET_FORMAT "{\"id\":\"123\",\"params\":{\"CH2O\":{\"value\":%d},\"power\":{\"value\":%d}}}"
#define ONENET_FORMAT "{\"id\":\"123\",\"params\":{\"humidity\":{\"value\":%.2f},\"temperature\":{\"value\":%.2f},\"CH2O\":{\"value\":%d},\"PM2-5\":{\"value\":%.2f}}}"


unsigned long lastMs = 0;

WiFiClient espClient;
PubSubClient client(espClient);


//连接wifi
void wifiInit()
{
  WiFi.begin(ssid,password);
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.println("Connectingto WiFi..");
  }
     Serial.println("Connectedto the WiFi network"); 
}


//mqtt连接
void mqttCheckConnect()
{
   client.setServer(mqttHostUrl,mqttPort);
  while (!client.connected()){
    Serial.println("Connectingto MQTT...");
    if (client.connect(clientId,mqttUser, mqttPassword )) {
      Serial.println("connected");
    }else {
      Serial.print("failedwith state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

}


//组数据帧并发送
void mqttIntervalPost()
{
    char param[256];
    char jsonBuf[128];
    int power=int(random(0,100));
    float t,h,d;
    int c;
   if (Serial.available()>0) { // 检查是否有数据可读

    h = Serial.parseFloat();
    Serial.println(h);

    t = Serial.parseFloat();
    Serial.println(t);

    c = Serial.parseInt(); 
    Serial.println(c);

    d = Serial.parseFloat(); 
    Serial.println(d);

  }
    // h=float((random(590,610))/10.0);
    // Serial.println(h);
    // t=float((random(240,260))/10.0);
    // Serial.println(t);
    // c=int(random(9,12));
    // Serial.println(c);
    // d=float((random(100,2000))/100.0);
    // Serial.println(d);

  
    //sprintf(jsonBuf,ONENET_FORMAT,power);
    sprintf(jsonBuf,ONENET_FORMAT,h,t,c,d);
    Serial.println(jsonBuf);
    client.publish(ONENET_POST, jsonBuf);
    delay(1000);
 
   
}


void setup() {
  Serial.begin(115200);
  wifiInit();//连接wifi
  mqttCheckConnect();//连接
}
 
void loop() {
  client.loop();
 
  if (millis() - lastMs >= 5000)
    {//定时检查或上报数据
        lastMs = millis();
        mqttCheckConnect(); 
        // 上报 
        mqttIntervalPost();
    }
}





