#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DHT.h>

SoftwareSerial wifiSerial(9,10);
DHT dht(2,DHT11);

// ===========配置区域==========


// ===========配置区域===========

bool doCmdOk(String data, char *keyword)
{
  bool result = false;
  if (data != "") // 对于tcp连接命令，直接等待第二次回复
  {
    wifiSerial.print(data); // 发送AT指令
    wifiSerial.print("\r\n"); 
  }
    // 等待模块回复
  while (!wifiSerial.available());
  delay(200);

  if (wifiSerial.find(keyword))        //返回值判断
    result = true;
  else  
    result = false;
  while (wifiSerial.available()) 
    wifiSerial.read();                 //清空串口接收缓存
  delay(500);                         //指令时间间隔
  return result;
}



void setup()
{
  Serial.begin(9600);
  Serial.println("begin");
  wifiSerial.begin(115200);  
  Serial.println("0");
  while (!doCmdOk(F("AT"), "OK"));
  Serial.println("1");
  while (!doCmdOk(F("AT+RST"), "OK"));
  Serial.println("1");
  while (!doCmdOk(F("AT+CWMODE=1"), "OK"));
  Serial.println("2");
  while(!doCmdOk(F("AT+CWDHCP=1,1"), "OK"));
  Serial.println("3");
  while(!doCmdOk(F("AT+CWJAP=\"iQOO 9\",\"zq020527\""), "OK"));
  Serial.println("4");
  while(!doCmdOk(F("AT+MQTTUSERCFG=0,1,\"Arduino\",\"74974l7RQM\",\"version=2018-10-31&res=products%2F74974l7RQM%2Fdevices%2FArduino&et=1741680668&method=md5&sign=pDijqtZlBe9j%2F5KJqSkmvA%3D%3D\",0,0,\"\""), "OK"));
  Serial.println("5");
  while(!doCmdOk(F("AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1"), "OK")); 
  Serial.println("6");
  while(!doCmdOk(F("AT+MQTTSUB=0,\"$sys/74974l7RQM/Arduino/thing/property/post/reply\",1"), "OK")); 
  Serial.println("7"); 
  // while(!doCmdOk(F("AT+MQTTPUB=0,\"$sys/74974l7RQM/Arduino/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"power\\\":{\\\"value\\\":23\\\}}}\",0,0"), "OK"));
  // Serial.println("8"); 

  dht.begin();
}

void loop()
{
  char s[256];
  // int a=random(0,100),b=random(0,100);
  delay(2000);
  char var=Serial.read();
  float h = dht.readHumidity();

  float t = dht.readTemperature();

  float f = dht.readTemperature(true);
  

  float hif = dht.computeHeatIndex(f, h);

  float hic = dht.computeHeatIndex(t, h, false);
  Serial.println(h);
  Serial.println("8"); 
  sprintf(s,"AT+MQTTPUB=0,\"$sys/74974l7RQM/Arduino/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"humidty\\\":{\\\"value\\\":%f}}}\",0,0\r\n",h);
  Serial.println("9"); 
  while(!doCmdOk(s, "OK"));
  Serial.println("8"); 

  if (wifiSerial.available())
    Serial.write(wifiSerial.read());
  if (Serial.available())
  {
    wifiSerial.write(Serial.read());
  }
}


