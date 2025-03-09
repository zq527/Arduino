int pinBuzzer = 3;                        //管脚D3连接到蜂鸣器模块的信号脚
 
void setup()
 {
  pinMode(pinBuzzer, OUTPUT);             //设置pinBuzzer脚为输出状态
}
 
void loop() 
{ 
   long frequency = 300;                   //频率, 单位Hz
 
                                          //用tone()函数发出频率为frequency的波形
   tone(pinBuzzer, frequency );
   delay(1000);                           //等待1000毫秒
   
   noTone(pinBuzzer);                    //停止发声
   delay(2000);                          //等待2000毫秒
}