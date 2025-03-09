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
#define ONENET_FORMAT "{\"id\":\"123\",\"params\":{\"humidity\":{\"value\":%f},\"temperature\":{\"value\":%f},\"CH2O\":{\"value\":%d},\"PM2-5\":{\"value\":%f}}}"

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
    //Serial.println(h);

    t = Serial.parseFloat();
    //Serial.println(t);

    c = Serial.parseInt(); 
    //Serial.println(c);

    d = Serial.parseFloat(); 
    //Serial.println(d);

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



