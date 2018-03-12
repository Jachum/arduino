#include <TimerOne.h>

const u8  led_1 = 13;
const u16 interval = 100*1000; //us

volatile bool light1  = HIGH;
volatile u16 sample  = 0;

void setup() 
{
  pinMode(led_1, OUTPUT);
  pinMode(A0, INPUT);
  
  Serial.begin(9600);

  //timer
  Timer1.initialize(interval);
  Timer1.attachInterrupt(timer_cb);
  //Timer1.stop();
  Timer1.start();
}

void loop() 
{
   Serial.print("Potentiometer reading: ");
   Serial.println(sample);
}


void timer_cb()
{
  light1 = !light1;
  digitalWrite(led_1, light1);
  sample = analogRead(A0);
}
