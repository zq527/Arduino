int measurePin = A5;            
int ledPower = 12;              
 
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;
 
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
 
void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  digitalWrite(ledPower,LOW);
}
 
void loop(){
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin);
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,LOW);
  delayMicroseconds(sleepTime);
 
  calcVoltage = voMeasured*5000/1024.0;
  dustDensity = 0.17*calcVoltage*2-0.1;  ///个人总结

  // calcVoltage = voMeasured * (5.0 / 1024);

 
  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }
 
  Serial.println("Raw Signal Value (0-1023):");
  Serial.println(voMeasured);
 
  Serial.println("Voltage:");
  Serial.println(calcVoltage);
 
  Serial.println("Dust Density:");
  Serial.println(dustDensity);
  Serial.println();
 
  delay(5000);
}