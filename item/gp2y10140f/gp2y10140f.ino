/*

【Arduino】168种传感器模块系列实验（资料代码+仿真编程+图形编程）

实验一百二十一：夏普SHARP PM2.5 灰尘/粉尘传感器 GP2Y1014AU0F 带线

项目：模块测试

GP2Y1014AU0F   Arduino Pin

1     Vled     –>  5V (150ohm resistor)

2     LED-GND –>  GND

3     LED      –>  Digital pin 2

4     S-GND    –>  GND

5     Vo       –>  Analog pin 0

6     Vcc      –>  5V

LED引脚必须调节成1ms的周期。

*/



#define dustPin A5

float dustVal = 0;

int ledPower = 12;

int delayTime = 280;

int delayTime2 = 40;

float offTime = 9680;



void setup() {

  Serial.begin(9600);

  pinMode(ledPower, OUTPUT);

  pinMode(dustPin, INPUT);
}



void loop() {

  // ledPower is any digital pin on the arduino connected to Pin 3 on the sensor

  digitalWrite(ledPower, LOW);

  delayMicroseconds(delayTime);

  dustVal = analogRead(dustPin);

  delayMicroseconds(delayTime2);

  digitalWrite(ledPower, HIGH);

  delayMicroseconds(offTime);

  delay(1000);

  Serial.println(dustVal);

  // if (dustVal > 36.455)

  // Serial.println((float(dustVal / 1024) - 0.0356) * 120000 * 0.035);
}
