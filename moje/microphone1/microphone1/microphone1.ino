#include <TimerOne.h>

const u8  led_1 = 13;

/*
 * To get fs=20kHz we need T=1/f => T=0.00005s => T=50us
 * @16MHz clock we've got a tick every 0.0625 us. This gives us
 * 800 ticks between every interrupt.
 */

const u16 interval = 50; //50us->800ticks, 100->1600ticks

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
   //Serial.print("Potentiometer reading: ");
   //Serial.println(sample);
}


void timer_cb()
{
  light1 = !light1;
  digitalWrite(led_1, light1);
  sample = analogRead(A0);
}
