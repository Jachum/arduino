#include <TimerOne.h>

const u8  led_1 = 13;
const u8  relay_pin = 7;
/*
   To get fs=20kHz we need T=1/f => T=0.00005s => T=50us
   @16MHz clock we've got a tick every 0.0625 us. This gives us
   800 ticks between every interrupt.
*/

const u16 interval = 50; //50us->800ticks, 100->1600ticks

volatile bool light1  = LOW;
volatile u16 sample  = 0;
volatile bool new_sample  = false;

void setup()
{
  pinMode(led_1, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(A0, INPUT);
  
  digitalWrite(led_1, light1);
  digitalWrite(relay_pin, light1);
      
  Serial.begin(9600);

  //timer
  Timer1.initialize(interval);
  Timer1.attachInterrupt(timer_cb);
  //Timer1.stop();
  Timer1.start();
}

void loop()
{
  const u16 zero = 512;
  const u16 thr = 300;
  const u16 thresholdUp = zero + thr;
  const u16 thresholdDwn = zero - thr;
  //Serial.print("Potentiometer reading: ");
  //Serial.println(sample);

  if (new_sample)
  {
    const u16 level = sample;
    if (level > thresholdUp || level < thresholdDwn)
    {
      light1 = !light1;
      digitalWrite(led_1, light1);
      digitalWrite(relay_pin, light1);
      Serial.println(sample);
    }
  }

}




void timer_cb()
{
  sample = analogRead(A0);
  new_sample = true;
  //Serial.println(sample);
}
